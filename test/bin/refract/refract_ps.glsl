#version 420

// layout(binding=0) uniform sampler2D DiffuseTexture;
// layout(binding=1) uniform sampler2D BumpTexture;
layout(binding=0) uniform samplerCube CubemapTextureSkybox;

in vec3 vo_position;
in vec3 vo_normal;
in vec2 vo_texcoord;
// in vec3 vo_tangent;
// in vec3 vo_bitangent;
in vec3 vo_viewdir;

out vec4 out_color;

vec3 LightDirection = vec3(-0.641413, -0.420927, -0.641413);
vec3 LightColor = vec3(1.0, 1.0, 1.0);
float LightIntensity = 1.0;

vec4 reflectRefract( vec3 L, vec3 N, vec3 V ) {
	float FresnelBias = 0.05;
	float FresnelScale = 0.42;
	float FresnelPow = 0.75;
	vec3 EtaRatio = vec3(0.66, 0.5, 0.57);

	float reflectionFactor = FresnelBias + FresnelScale * pow(1.0 + dot(V, N), FresnelPow);
	vec3 R = reflect(V, N);
	vec3 Red = refract(V, N, EtaRatio.r);
	vec3 Green = refract(V, N, EtaRatio.g);
	vec3 Blue = refract(V, N, EtaRatio.b);

	vec4 reflectedColor = texture(CubemapTextureSkybox, R);
	vec4 refractedColor;
	refractedColor.r = texture(CubemapTextureSkybox, Red).r;
	refractedColor.g = texture(CubemapTextureSkybox, Green).g;
	refractedColor.b = texture(CubemapTextureSkybox, Blue).b;
	refractedColor.a = reflectionFactor;

	vec4 final = mix(refractedColor, reflectedColor, reflectionFactor);
	return final;
}

vec3 phong( vec3 L, vec3 N, vec3 V, vec3 color, float intensity ) {
	float nDotL = max(dot(N, L), 0.0);
	vec3 RL = normalize(2.0 * N * nDotL - L);
	float rDotV = max(dot(RL, V), 0.0);
	float SPEC_POW = 32.0;
	vec3 specular;
	if( SPEC_POW > 0.0 ) {
		specular = color * pow(rDotV, SPEC_POW);
	}
	vec3 diffuse = color * nDotL;
	return (diffuse + specular) * intensity;
}

vec3 ward( vec3 L, vec3 N, vec3 V, vec3 color, float intensity, float roughness ) {
	vec3 H = normalize(L + V);
	float vDotN = dot(V, N);
	float lDotN = dot(L, N);
	float hDotN = dot(H, N);
	float r_sq = (roughness * roughness) + 1e-5;
	float exp_a = -pow(tan(acos(hDotN)), 2.0);
	float spec_num = exp(exp_a / r_sq);
	float spec_den = 4.0 * 3.14159 * r_sq;
  spec_den *= sqrt( lDotN * vDotN );
  vec3 specular = color * max(0.0, (spec_num / spec_den));
  vec3 diffuse = color;
  return vec3(lDotN * (diffuse + specular)) * intensity;
}

void main() {
	// light direction
	vec3 L = normalize(-LightDirection);

	// camera view vector
	vec3 V = normalize(vo_viewdir);

	// extract and convert normal
	vec3 N = normalize(vo_normal);

	// #define PHONG
	// #define WARD

	// #ifdef PHONG
	// out_color = vec4(phong(L, N, V, LightColor, LightIntensity), 1.0);
	// #endif

	// #ifdef WARD
	// out_color = vec4(ward(L, N, V, LightColor, LightIntensity, 0.2), 1.0);
	// #endif

	vec4 reflRefr = reflectRefract(L, N, V);
	vec3 lighting = ward(L, N, V, LightColor, LightIntensity, 0.6);

	out_color = vec4(lighting * reflRefr.rgb, 1.0);//reflRefr.a);
}
