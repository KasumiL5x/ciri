#version 440

uniform sampler2D DepthTexture;

in vec3 vo_wpos;
in vec3 vo_wnrm;
in vec3 vo_campos;
in vec4 vo_wposLightSpace;

out vec4 out_color;

vec3 AmbientLightColor = vec3(0.05333332, 0.09882354, 0.1819608);
vec3 DirLightDirection = vec3(-0.5265408, -0.5735765,-0.6275069);
vec3 DirLightDiffuseColor = vec3(1.0, 0.9607844, 0.8078432);

vec3 phong( vec3 L, vec3 N, vec3 lightColor, float lightIntensity, float specularPower, vec3 worldPos, vec3 camPos ) {
	float nDotL = max(dot(N, L), 0.0);
	vec3 RL = normalize(2.0 * N * nDotL - L);
	vec3 V = normalize(camPos - worldPos);
	float rDotV = max(dot(RL, V), 0.0);
	vec3 specular = vec3(0.0);
	if( specularPower > 0.0 ) {
		specular = lightColor * pow(rDotV, specularPower);
	}
	vec3 diffuse = lightColor * nDotL;
	return (diffuse + specular) * lightIntensity;
}

float D2R( float d ) {
	return d * 0.0174532924;
}

vec3 spotlight() {
	vec3 LightPosition = vec3(-10.0f, 40.0f, -10.0f);
	vec3 LightTarget = vec3(20.0, 0.0, 20.0);
	vec3 LightDirection = normalize(LightTarget - LightPosition); //vec3(-0.5f, -0.99f, 0.0f));
	float LightCosInnerAngle = cos(D2R(30.0));
	float LightCosOuterAngle = cos(D2R(35.0));
	float LightRange = 150.0;
	vec3 LightDiffuseColor = vec3(1.0, 1.0, 1.0);
	float LightDiffuseIntensity = 1.0;
	float SpecularPower = 128.0;
	vec3 ObjColor = vec3(1.0, 1.0, 1.0);


	vec3 L = LightPosition - vo_wpos;
	float distToLight = length(L);
	L = normalize(L);
	float cosDir = dot(L, -LightDirection);
	float spotEffect = smoothstep(LightCosOuterAngle, LightCosInnerAngle, cosDir);
	float heightAttenuation = smoothstep(LightRange, 0.0, distToLight);
	vec3 N = normalize(vo_wnrm);
	float diffuseLight = max(dot(N, L), 0.0);
	vec3 diffuse = (diffuseLight * LightDiffuseColor) * LightDiffuseIntensity;
	float specularLight = 0.0;
	if( SpecularPower > 0.0 ) {
		vec3 V = normalize(vo_campos - vo_wpos);
		vec3 H = normalize(L+V);
		vec3 R = reflect(-L, N);
		specularLight = pow(clamp(dot(R, H), 0.0, 1.0), SpecularPower);
	}
	vec3 specular = (specularLight * LightDiffuseColor) * LightDiffuseIntensity;
	vec3 finalLightColor = ((diffuse + specular) * ObjColor) * spotEffect * heightAttenuation;


	// shadowing
	float visibility = 1.0;
	vec3 screenPos = vo_wposLightSpace.xyz / vo_wposLightSpace.w;
	screenPos = screenPos * 0.5 + 0.5;
	if( screenPos.z > 1.0 ) {
		visibility = 1.0;
	} else {
		// float shadowSample = texture(DepthTexture, screenPos.xy).x;
		// visibility = (shadowSample < screenPos.z - 0.001) ? 0.5 : 1.0;

		vec2 poissonDisk[4] = vec2[](
			vec2( -0.94201624, -0.39906216 ),
			vec2( 0.94558609, -0.76890725 ),
			vec2( -0.094184101, -0.92938870 ),
			vec2( 0.34495938, 0.29387760 )
		);
		for( int i = 0; i < 4; ++i ) {
			vec2 coord = screenPos.xy + poissonDisk[i]/400.0;
			if( texture(DepthTexture, coord).x < screenPos.z-0.001 ) {
				visibility -= 0.2;
			}
		}
	}

	// return vec3(screenPos.z);
	// return vec3(shadow);

	// return vec3(texture(DepthTexture, screenPos.xy).xyz);
	// return vec3(shadow);
	return finalLightColor * visibility;
	// return finalLightColor;
}

void main() {
	// vec3 L = -DirLightDirection;
	// vec3 N = normalize(vo_wnrm);
	// vec3 lighting = AmbientLightColor + phong(L, N, DirLightDiffuseColor, 1.0, 128.0, vo_wpos, vo_campos);
	// out_color = vec4(lighting, 1.0);

	out_color = vec4(spotlight(), 1.0);
	// out_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
