#version 330

uniform sampler2D TestTexture;

in vec3 vo_position;
in vec3 vo_normal;
in vec2 vo_texcoord;

out vec4 out_color;

vec3 AmbientLightColor = vec3(0.05333332f, 0.09882354f, 0.1819608f);
vec3 LightDirection = vec3(-0.5265408f, -0.5735765f,-0.6275069f);
vec3 LightColor = vec3(1.0f, 0.9607844f, 0.8078432f);

vec3 lambert( vec3 L, vec3 N, vec3 lightColor, float lightIntensity ) {
	return (max(dot(L, N), 0.0f) * lightColor) * lightIntensity;
}

vec3 phong( vec3 L, vec3 N, vec3 lightColor, float lightIntensity, float specularPower, vec3 worldPos, vec3 cameraPos ) {
	float nDotL = max(dot(L, N), 0.0f);
	vec3 RL = normalize(2.0f * N * nDotL - L);
	vec3 V = normalize(cameraPos - worldPos);
	float rDotV = max(dot(RL, V), 0.0f);
	vec3 specular = vec3(0.0f);
	if( specularPower > 0.0f ) {
		specular = lightColor * pow(rDotV, specularPower);
	}
	return (lightColor * nDotL + specular) * lightIntensity;
}

void main() {
	// lighting
	vec3 L = -LightDirection;
	vec3 N = normalize(vo_normal);
	vec3 lighting = AmbientLightColor + lambert(L, N, LightColor, 1.0f);
	// texture sample
	vec3 textureSample = texture(TestTexture, vo_texcoord).rgb;
	// final
	out_color = vec4(textureSample * lighting, 1.0f);
}
