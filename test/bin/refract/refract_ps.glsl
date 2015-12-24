#version 420

layout(binding=0) uniform samplerCube CubemapTexture;
// layout(binding=0) uniform sampler3D CubemapTexture;

in vec3 vo_position;
in vec3 vo_normal;
in vec2 vo_texcoord;
in vec3 vo_viewdir;

out vec4 out_color;

vec3 LightDirection = vec3(-0.5265408f, -0.5735765f,-0.6275069f);
vec4 DiffuseColor = vec4(1.0, 1.0, 1.0, 1.0);
vec4 AmbientColor = vec4(0.5, 0.5, 0.5, 1.0);

vec3 lambert( vec3 L, vec3 N, vec3 lightColor, float lightIntensity ) {
	return (max(dot(L, N), 0.0f) * lightColor) * lightIntensity;
}

void main() {
	vec3 L = normalize(-LightDirection);
	vec3 N = normalize(vo_normal);

	float diffuseLight = max(dot(L, N), 0.0);

	float AIR = 1.00029;
	float ICE = 1.31;
	float RIDX = AIR / ICE;

	vec3 V = normalize(vo_viewdir);
	vec3 Refract = refract(V, N, RIDX);
	vec3 refractColor = texture(CubemapTexture, Refract).rgb;

	out_color = vec4(AmbientColor * vec4(refractColor, 1.0) + DiffuseColor * diffuseLight);

	// out_color = texture(CubemapTexture, vec3(vo_texcoord, diffuseLight));
}
