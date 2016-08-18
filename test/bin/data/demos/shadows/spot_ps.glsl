#version 420

uniform sampler2D DepthTexture;

in vec3 vo_wpos;
in vec3 vo_wnrm;
in vec3 vo_campos;
in vec3 vo_LightPosition;
in vec3 vo_LightDirection;
in vec3 vo_LightColor;
in float vo_LightCosInner;
in float vo_LightCosOuter;
in float vo_LightIntensity;
in float vo_LightRange;
flat in int vo_CastShadows;
in vec4 vo_wposLightSpace;

out vec4 out_color;

float random(vec3 seed, int i) {
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

void main() {
	// light vector and distance to light
	vec3 L = vo_LightPosition - vo_wpos;
	float distToLight = length(L);
	L = normalize(L);

	// spotlight affect
	float cosDir = dot(L, -vo_LightDirection);
	float spotEffect = smoothstep(vo_LightCosOuter, vo_LightCosInner, cosDir);
	float heightAttenuation = smoothstep(vo_LightRange, 0.0, distToLight);

	// lighting
	vec3 N = normalize(vo_wnrm);
	float diffuseLight = max(dot(N, L), 0.0);
	vec3 diffuse = (diffuseLight * vo_LightColor) * vo_LightIntensity;
	float specularLight = 0.0;
	float SPECPOW = 48.0;
	if( SPECPOW > 0.0 ) {
		vec3 V = normalize(vo_campos - vo_wpos);
		vec3 H = normalize(L+V);
		vec3 R = reflect(-L, N);
		specularLight = pow(clamp(dot(R, H), 0.0, 1.0), SPECPOW);
	}
	vec3 specular = (specularLight * vec3(1.0)) * vo_LightIntensity;
	vec3 ObjColor = vec3(1.0);
	vec3 lighting = ((diffuse + specular) * ObjColor) * spotEffect * heightAttenuation;

	float visibility = 1.0;
	if( vo_CastShadows != 0 ) {
		vec3 screenPos = vo_wposLightSpace.xyz / vo_wposLightSpace.w;
		screenPos = screenPos * 0.5 + 0.5;
		if( screenPos.z > 1.0 ) {
			visibility = 1.0;
		} else {
			float bias = clamp(0.005 * tan(acos(diffuseLight)), 0.0, 0.00005);
			// one sample
			// float depthSample = texture(DepthTexture, screenPos.xy).x;
			// visibility = depthSample < (screenPos.z - bias) ? 0.0 : visibility;

			// pcf
			float shadowFactor = 0.0;
			vec2 texelSize = 1.0 / textureSize(DepthTexture, 0);
			for( int y = -1; y <= 1; ++y ) {
				for( int x = -1; x <= 1; ++x ) {
					vec2 offset = vec2(x, y) * texelSize;
					shadowFactor += texture(DepthTexture, screenPos.xy + offset).r < (screenPos.z - bias) ? 1.0f : 0.0f;
				}
			}
			shadowFactor /= 9.0;
			visibility = 1.0f-shadowFactor;

			// // stratified poisson sampling
			// vec2 poissonDisk[16] = vec2[]( 
			// 	vec2( -0.94201624, -0.39906216 ), 
			// 	vec2( 0.94558609, -0.76890725 ), 
			// 	vec2( -0.094184101, -0.92938870 ), 
			// 	vec2( 0.34495938, 0.29387760 ), 
			// 	vec2( -0.91588581, 0.45771432 ), 
			// 	vec2( -0.81544232, -0.87912464 ), 
			// 	vec2( -0.38277543, 0.27676845 ), 
			// 	vec2( 0.97484398, 0.75648379 ), 
			// 	vec2( 0.44323325, -0.97511554 ), 
			// 	vec2( 0.53742981, -0.47373420 ), 
			// 	vec2( -0.26496911, -0.41893023 ), 
			// 	vec2( 0.79197514, 0.19090188 ), 
			// 	vec2( -0.24188840, 0.99706507 ), 
			// 	vec2( -0.81409955, 0.91437590 ), 
			// 	vec2( 0.19984126, 0.78641367 ), 
			// 	vec2( 0.14383161, -0.14100790 ) 
			// );
			// for( int i = 0; i < 4; ++i ) {
			// 	int index = int(16.0 * random(gl_FragCoord.xyy, i)) % 16;
			// 	visibility -= texture(DepthTexture, vec2(screenPos.xy + poissonDisk[index]/400.0)).r < screenPos.z - bias ? 0.2 : 0.0;
			// }
		}
	}

	out_color = vec4(lighting * visibility, 1.0);
}
