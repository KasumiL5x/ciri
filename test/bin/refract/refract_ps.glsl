#version 420

layout(binding=0) uniform sampler2D DiffuseTexture;
layout(binding=1) uniform sampler2D BumpTexture;
layout(binding=2) uniform samplerCube CubemapTextureSkybox;

in vec3 vo_position;
in vec3 vo_normal;
in vec2 vo_texcoord;
in vec3 vo_tangent;
in vec3 vo_bitangent;
in vec3 vo_viewdir;

out vec4 out_color;

vec3 LightDirection = vec3(-0.641413, -0.420927, -0.641413);

void main() {
	vec3 L = normalize(-LightDirection);

	vec3 N = texture(BumpTexture, vo_texcoord).rgb;
	N = normalize(N * 2.0 - 1.0);
	N = (N.x * vo_tangent) + (N.y * vo_bitangent) + (N.z * vo_normal);
	N = normalize(N);
	// N = normalize(vo_normal);

	float diffuseFactor = max(dot(L, N), 0.0f);
	vec3 diffuse = vec3(1.0, 1.0, 1.0) * diffuseFactor;

	float SPEC_POW = 32.0;
	vec3 RL = normalize(2.0 * N * diffuseFactor - L);
	vec3 V = normalize(vo_viewdir);
	float rDotV = max(dot(RL, V), 0.0);
	vec3 specular;
	if( SPEC_POW > 0.0 ) {
		specular = vec3(1.0, 1.0, 1.0) * 0.7 * pow(rDotV, SPEC_POW);
	}

	// sample diffuse texture
	vec3 albedo = texture(DiffuseTexture, vo_texcoord).rgb;

	vec3 final =
		// ambient
		(albedo * vec3(0.1, 0.1, 0.1)) +
		// diffuse color and lighting
		(albedo * diffuse) +
		// specular reflection
		(specular);
	out_color = vec4(final, 1.0);

	// out_color = vec4( * diffuse + specular, 1.0);

	// // can move into VS:
	// float FresnelBias = 0.0;
	// float FresnelScale = 0.42;
	// float FresnelPow = 0.75;
	// vec3 EtaRatio = vec3(0.66, 0.5, 0.57);
	// //
	// vec3 L = normalize(-LightDirection);
	// vec3 N = normalize(vo_normal);
	// vec3 V = normalize(vo_viewdir);
	// //
	// vec3 R = reflect(V, N);
	// vec3 TRed = refract(V, N, EtaRatio.r);
	// vec3 TGreen = refract(V, N, EtaRatio.g);
	// vec3 TBlue = refract(V, N, EtaRatio.b);
	// //
	// float reflectionFactor = FresnelBias + FresnelScale * pow(1.0 + dot(V, N), FresnelPow);

	// // can stay in ps:
	// vec4 reflectedColor = texture(CubemapTextureSkybox, R);
	// vec4 refractedColor;
	// refractedColor.r = texture(CubemapTextureSkybox, TRed).r;
	// refractedColor.g = texture(CubemapTextureSkybox, TGreen).g;
	// refractedColor.b = texture(CubemapTextureSkybox, TBlue).b;
	// refractedColor.a = 1.0;
	// vec4 final = mix(refractedColor, reflectedColor, reflectionFactor);
	// // final.a = reflectionFactor;
	// out_color = final;

	// // out_color = vec4(vo_texcoord.x, vo_texcoord.x, vo_texcoord.x, 1.0);
	// // out_color = texture(BumpTexture, vo_texcoord);
}
