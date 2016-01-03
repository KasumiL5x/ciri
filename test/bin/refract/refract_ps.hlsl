// Texture2D DiffuseTexture : register(t0);
// Texture2D BumpTexture : register(t1);
TextureCube CubemapTextureSkybox : register(t0);
SamplerState CubemapSampler : register(s0);

struct Input {
	float4 hpos : SV_POSITION;
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float2 texcoord : TEXCOORD0;
	// float3 tangent : TEXCOORD1;
	// float3 bitangent : TEXCOORD2;
	float3 viewdir : TEXCOORD3;
};

static float3 LightDirection = float3(-0.641413, -0.420927, -0.641413);
static float3 LightColor = float3(1.0, 1.0, 1.0);
static float LightIntensity = 1.0;

float4 reflectRefract( float3 L, float3 N, float3 V ) {
	float FresnelBias = 0.05;
	float FresnelScale = 0.42;
	float FresnelPow = 0.75;
	float3 EtaRatio = float3(0.66, 0.5, 0.57);

	float reflectionFactor = FresnelBias + FresnelScale * pow(1.0 + dot(V, N), FresnelPow);
	float3 R = reflect(V, N);
	float3 Red = refract(V, N, EtaRatio.r);
	float3 Green = refract(V, N, EtaRatio.g);
	float3 Blue = refract(V, N, EtaRatio.b);

	float4 reflectedColor = CubemapTextureSkybox.Sample(CubemapSampler, R);
	float4 refractedColor;
	refractedColor.r = CubemapTextureSkybox.Sample(CubemapSampler, Red).r;
	refractedColor.g = CubemapTextureSkybox.Sample(CubemapSampler, Green).g;
	refractedColor.b = CubemapTextureSkybox.Sample(CubemapSampler, Blue).b;
	refractedColor.a = reflectionFactor;

	float4 final = lerp(refractedColor, reflectedColor, reflectionFactor);
	return final;
}

float3 phong( float3 L, float3 N, float3 V, float3 color, float intensity ) {
	float nDotL = max(dot(N, L), 0.0);
	float3 RL = normalize(2.0 * N * nDotL - L);
	float rDotV = max(dot(RL, V), 0.0);
	float SPEC_POW = 32.0;
	float3 specular;
	if( SPEC_POW > 0.0 ) {
		specular = color * pow(rDotV, SPEC_POW);
	}
	float3 diffuse = color * nDotL;
	return (diffuse + specular) * intensity;
}

float3 ward( float3 L, float3 N, float3 V, float3 color, float intensity, float roughness ) {
	float3 H = normalize(L + V);
	float vDotN = dot(V, N);
	float lDotN = dot(L, N);
	float hDotN = dot(H, N);
	float r_sq = (roughness * roughness) + 1e-5;
	float exp_a = -pow(tan(acos(hDotN)), 2.0);
	float spec_num = exp(exp_a / r_sq);
	float spec_den = 4.0 * 3.14159 * r_sq;
  spec_den *= sqrt( lDotN * vDotN );
  float3 specular = color * max(0.0, (spec_num / spec_den));
  float3 diffuse = color;
  return float3(lDotN * (diffuse + specular)) * intensity;
}

float4 main( Input input ) : SV_Target {
	// light direction
	float3 L = normalize(-LightDirection);

	// camera view vector
	float3 V = normalize(input.viewdir);

	// extract and convert normal
	float3 N = normalize(input.normal);
	
	// #define PHONG
	// #define WARD

	// #ifdef PHONG
	// return float4(albedo * phong(L, N, V, LightColor, LightIntensity), 1.0);
	// #endif

	// #ifdef WARD
	// return float4(albedo * ward(L, N, V, LightColor, LightIntensity, 0.2), 1.0);
	// #endif

	float4 reflRefr = reflectRefract(L, N, V);
	float3 lighting = ward(L, N, V, LightColor, LightIntensity, 0.6);
	return float4(lighting * reflRefr.rgb, 1.0);//reflRefr.a);
}
