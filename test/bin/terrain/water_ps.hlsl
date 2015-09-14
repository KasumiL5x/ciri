Texture2D NormalMapTex : register(t0);
SamplerState NormalMapSampler : register(s0);
Texture2D ReflectionTexture : register(t1);
SamplerState ReflectionSampler : register(s1);
Texture2D RefractionTexture : register(t2);
SamplerState RefractionSampler : register(s2);

struct Input {
	float4 hpos          : SV_POSITION;
	float3 position      : TEXCOORD1;
	float3 normal        : NORMAL0;
	float3 camPos        : TEXCOORD2;
	float2 bumpTexcoords : TEXCOORD3;
	float3 cubeTexcoords : TEXCOORD4;
	float waveHeight     : TEXCOORD5;
	float4 reflectionTexcoords : TEXCOORD6;
	float4 refractionTexcoords : TEXCOORD7;
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

	// perturbed texcoords
	float4 bumpColor = NormalMapTex.Sample(NormalMapSampler, input.bumpTexcoords);
	float3 perturb = input.waveHeight * (bumpColor.xyz - 0.5f) * 2.0f;

	// scene reflection
	float2 reflectionCoords;
	reflectionCoords.x =  input.reflectionTexcoords.x / input.reflectionTexcoords.w / 2.0f + 0.5f;// * 0.5f + 0.5f;
	reflectionCoords.y = -input.reflectionTexcoords.y / input.reflectionTexcoords.w / 2.0f + 0.5f;// * 0.5f + 0.5f;
	reflectionCoords += perturb.xy;
	float4 reflectionColor = ReflectionTexture.Sample(ReflectionSampler, reflectionCoords);

	// scene refraction
	float2 refractionCoords;
	refractionCoords.x =  input.refractionTexcoords.x / input.refractionTexcoords.w / 2.0f + 0.5f;
	refractionCoords.y = -input.refractionTexcoords.y / input.refractionTexcoords.w / 2.0f + 0.5f;
	refractionCoords += perturb.xy;
	float4 refractionColor = RefractionTexture.Sample(ReflectionSampler, refractionCoords);

	// fresnel factor
	float3 eyeVector = normalize(input.camPos - input.position);
	float3 normalVector = input.normal;
	float fresnelTerm = max(0.0f, dot(eyeVector, normalVector));
	float4 combinedReflRefr = lerp(reflectionColor, refractionColor, fresnelTerm);
	float4 combinedColor = lerp(float4(sunlight, 1.0f), float4(combinedReflRefr.xyz, 1.0f), 0.85f);

	// coloration
	float4 dullColor = float4(0.3f, 0.3f, 0.5f, 1.0f);

	float4 out_color = lerp(combinedColor, dullColor, 0.2f);
	out_color.w = 0.75f;
	return out_color;
}
