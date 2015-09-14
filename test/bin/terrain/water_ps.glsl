#version 420

in vec3 vo_position;
in vec3 vo_normal;
in vec3 vo_campos;
in vec2 vo_bumpTexcoords;
in vec3 vo_cubeTexcoords;
in float vo_waveHeight;
in vec4 vo_reflectionTexcoords;
in vec4 vo_refractionTexcoords;

out vec4 out_color;

layout(binding=0) uniform sampler2D NormalMap;
layout(binding=1) uniform sampler2D ReflectionTexture;
layout(binding=2) uniform sampler2D RefractionTexture;

// -*- Light properties -*-
vec3 LightDirection = vec3(-0.5265408f, -0.5735765f,-0.6275069f);

// -*- Sunlight properties -*-
float SunStrength = 1.2f;
float SunShine = 75.0f;
vec3 SunColor = vec3(1.2f, 0.4f, 0.1f);
float SunPow = 0.45454545f;

vec3 calcSunlight( vec3 RV, vec3 L ) {
	return pow(SunStrength * pow(max(0.0, dot(RV, L)), SunShine) * SunColor, vec3(SunPow));
}

void main() {
	// sunlight
	vec3 L = normalize(-LightDirection);
	vec3 N = texture(NormalMap, vo_bumpTexcoords).xyz; // needs tbn?
	vec3 V = normalize(vo_position - vo_campos);
	vec3 RV = normalize(reflect(V, N));
	vec3 sunlight = calcSunlight(RV, L);

	// perturbed texcoords
	vec4 bumpColor = texture(NormalMap, vo_bumpTexcoords);
	vec3 perturb = vo_waveHeight * (bumpColor.xyz - 0.5f) * 2.0f;

	// scene reflection
	vec2 reflectionCoords;
	reflectionCoords.x =  vo_reflectionTexcoords.x / vo_reflectionTexcoords.w * 0.5f + 0.5f;
	reflectionCoords.y = vo_reflectionTexcoords.y / vo_reflectionTexcoords.w * 0.5f + 0.5f;
	reflectionCoords += perturb.xy;
	vec4 reflectionColor = texture(ReflectionTexture, reflectionCoords);

	// scene refraction
	vec2 refractionCoords;
	refractionCoords.x =  vo_refractionTexcoords.x / vo_refractionTexcoords.w * 0.5f + 0.5f;
	refractionCoords.y = vo_refractionTexcoords.y / vo_refractionTexcoords.w * 0.5f + 0.5f;
	refractionCoords += perturb.xy;
	vec4 refractionColor = texture(RefractionTexture, refractionCoords);

	// fresnel factor
	vec3 eyeVector = normalize(vo_campos - vo_position);
	vec3 normalVector = vo_normal;
	float fresnelTerm = max(0.0f, dot(eyeVector, normalVector));
	vec4 combinedReflRefr = mix(reflectionColor, refractionColor, fresnelTerm);
	vec4 combinedColor = mix(vec4(sunlight, 1.0f), vec4(combinedReflRefr.xyz, 1.0f), 0.85f);

	// coloration
	vec4 dullColor = vec4(0.3f, 0.3f, 0.5f, 1.0f);

	out_color = mix(combinedColor, dullColor, 0.2f);
	out_color.w = 0.75f;
}
