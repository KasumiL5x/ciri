#version 330

in vec3 vo_position;
in vec3 vo_normal;
in vec2 vo_texcoord;
in vec3 vo_campos;

out vec4 out_color;

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
	vec3 L = -LightDirection;
	vec3 N = normalize(vo_normal);
	vec3 V = normalize(vo_position - vo_campos);
	vec3 RV = normalize(reflect(V, N));

	vec3 sunlight = calcSunlight(RV, L);

	vec3 water_color = vec3(0.25f, 0.64f, 0.87f);

	out_color = vec4(water_color + sunlight, 1.0f);
	out_color.x = min(1.0f, out_color.x);
	out_color.y = min(1.0f, out_color.y);
	out_color.z = min(1.0f, out_color.z);
}
