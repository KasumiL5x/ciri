Texture2D NormalMapTex : register(t0);
SamplerState NormalMapSampler : register(s0);
TextureCube SkyboxTex : register(t1);
SamplerState SkyboxSampler : register(s1);

struct Input {
	float4 hpos          : SV_POSITION;
	float3 position      : TEXCOORD1;
	float3 normal        : NORMAL0;
	float3 camPos        : TEXCOORD2;
	float2 bumpTexcoords : TEXCOORD3;
	float3 cubeTexcoords : TEXCOORD4;
	float waveHeight     : TEXCOORD5;
};

// -*- Light properties -*-
static float3 LightDirection = float3(-0.5265408f, -0.5735765f,-0.6275069f);

// -*- Sunlight properties -*-
static float SunStrength = 1.2f;
static float SunShine = 75.0f;
static float3 SunColor = float3(1.2f, 0.4f, 0.1f);
static float SunPow = 0.45454545f;

float3 calcSunlight( float3 RV, float3 L ) {
	return pow(SunStrength * pow(max(0.0f, dot(RV, L)), SunShine) * SunColor, SunPow);
}

float4 main( Input input ) : SV_Target {
	// sunlight
	float3 L = normalize(-LightDirection);
	float3 N = NormalMapTex.Sample(NormalMapSampler, input.bumpTexcoords).xyz;
	float3 V = normalize(input.position - input.camPos);
	float3 RV = normalize(reflect(V, N));
	float3 sunlight = calcSunlight(RV, L);

	// skybox reflection
	float4 bumpColor = NormalMapTex.Sample(NormalMapSampler, input.bumpTexcoords);
	float3 perturb = input.waveHeight * (bumpColor.xyz - 0.5f) * 2.0f;
	float3 perturbCubeCoords = input.cubeTexcoords + perturb;
	float3 cubemap = SkyboxTex.Sample(SkyboxSampler, perturbCubeCoords).xyz;

	// fresnel factor
	float3 eyeVector = normalize(input.camPos - input.position);
	float3 normalVector = input.normal;
	float fresnelTerm = max(0.0f, dot(eyeVector, normalVector));
	float4 combinedColor = lerp(float4(cubemap, 1.0f), float4(sunlight, 1.0f), fresnelTerm);

	// coloration
	// float4 dullColor = float4(0.77f, 0.90f, 0.92f, 1.0f);
	float4 dullColor = float4(0.3f, 0.3f, 0.5f, 1.0f);

	float4 out_color = lerp(combinedColor, dullColor, 0.2f);
	out_color.w = 0.75f;
	return out_color;
}
