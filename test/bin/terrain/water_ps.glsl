#version 330

in vec3 vo_position;
in vec3 vo_normal;
in vec3 vo_campos;
in vec2 vo_bumpTexcoords;
in vec3 vo_cubeTexcoords;
in float vo_waveHeight;

out vec4 out_color;

uniform sampler2D NormalMap;
uniform samplerCube SkyboxTexture;

// -*- Light properties -*-
vec3 LightDirection = vec3(-0.5265408f, -0.5735765f,-0.6275069f);

// -*- Sunlight properties -*-
float SunStrength = 1.2f;
float SunShine = 75.0f;
vec3 SunColor = vec3(1.0f, 0.9f, 0.85f);
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

	// skybox reflection
	vec4 bumpColor = texture(NormalMap, vo_bumpTexcoords);
	vec3 perturb = vo_waveHeight * (bumpColor.xyz - 0.5f) * 2.0f;
	vec3 perturbCubeCoords = vo_cubeTexcoords + perturb;
	vec3 cubemap = texture(SkyboxTexture, perturbCubeCoords).xyz;

	// fresnel factor
	vec3 eyeVector = normalize(vo_campos - vo_position);
	vec3 normalVector = vo_normal;
	float fresnelTerm = max(0.0f, dot(eyeVector, normalVector));
	vec4 combinedColor = mix(vec4(cubemap, 1.0f), vec4(sunlight, 1), fresnelTerm);

	// coloration
	// vec4 dullColor = vec4(0.77f, 0.90f, 0.92f, 1.0f);
	vec4 dullColor = vec4(0.3f, 0.3f, 0.5f, 1.0f);

	out_color = mix(combinedColor, dullColor, 0.2f);
	out_color.w = 0.75f;
}
