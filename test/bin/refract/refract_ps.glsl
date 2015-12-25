#version 420

layout(binding=0) uniform sampler3D CubemapTexture;
layout(binding=1) uniform samplerCube CubemapTextureSkybox;

in vec3 vo_position;
in vec3 vo_normal;
in vec2 vo_texcoord;
in vec3 vo_viewdir;

out vec4 out_color;

vec3 LightDirection = vec3(-0.5265408f, -0.5735765f,-0.6275069f);

void main() {
	// can move into VS:
	float FresnelBias = 0.0;
	float FresnelScale = 0.42;
	float FresnelPow = 0.75;
	vec3 EtaRatio = vec3(0.66, 0.5, 0.57);
	//
	vec3 L = normalize(-LightDirection);
	vec3 N = normalize(vo_normal);
	vec3 V = normalize(vo_viewdir);
	//
	vec3 R = reflect(V, N);
	vec3 TRed = refract(V, N, EtaRatio.r);
	vec3 TGreen = refract(V, N, EtaRatio.g);
	vec3 TBlue = refract(V, N, EtaRatio.b);
	//
	float reflectionFactor = FresnelBias + FresnelScale * pow(1.0 + dot(V, N), FresnelPow);

	// can stay in ps:
	vec4 reflectedColor = texture(CubemapTextureSkybox, R);
	vec4 refractedColor;
	refractedColor.r = texture(CubemapTextureSkybox, TRed).r;
	refractedColor.g = texture(CubemapTextureSkybox, TGreen).g;
	refractedColor.b = texture(CubemapTextureSkybox, TBlue).b;
	refractedColor.a = 1.0;
	vec4 final = mix(refractedColor, reflectedColor, reflectionFactor);
	// final.a = reflectionFactor;
	out_color = final;
}
