#version 420

uniform sampler2D DepthTexture;

in vec3 vo_wpos;
in vec3 vo_wnrm;
in vec3 vo_campos;
in vec3 vo_LightDirection;
in vec3 vo_LightColor;
in float vo_LightIntensity;
flat in int vo_CastShadows;
in vec4 vo_wposLightSpace;

out vec4 out_color;


void main() {
	vec3 L = normalize(-vo_LightDirection);
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
	vec3 lighting = ((diffuse + specular) * ObjColor);

	float visibility = 1.0;
	if( vo_CastShadows != 0 ) {
		vec3 screenPos = vo_wposLightSpace.xyz / vo_wposLightSpace.w;
		screenPos = screenPos * 0.5 + 0.5;
		if( screenPos.z > 1.0 ) {
			visibility = 1.0;
		} else {
			float bias = clamp(0.005 * tan(acos(diffuseLight)), 0.0, 0.006);
			// float depthSample = texture(DepthTexture, screenPos.xy).x;
			// visibility = depthSample < (screenPos.z - bias) ? 0.0 : visibility;

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

			
		}
	}

	out_color = vec4(lighting * visibility, 1.0);
}
