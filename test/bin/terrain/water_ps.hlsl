Texture2D NormalMapTex : register(t0);
SamplerState NormalMapSampler : register(s0);

struct Input {
	float4 hpos : SV_POSITION;
	float3 wpos : TEXCOORD1;
	float3 nrm : NORMAL0;
	float2 tex : TEXCOORD0;
	float3 campos : TEXCOORD2;
	float3x3 tbn : TEXCOORD3;
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
	float3 L = -LightDirection;
	float3 N = mul(input.tbn, normalize(NormalMapTex.Sample(NormalMapSampler, input.tex).xyz * 2.0f - 1.0f));
	float3 V = normalize(input.wpos - input.campos);
	float3 RV = normalize(reflect(V, N));

	float3 sunlight = calcSunlight(RV, L);

	float3 water_color = float3(0.25f, 0.64f, 0.87f);

	float light = max(dot(L, N), 0.0f);

	float4 out_color = float4((water_color + sunlight), 1.0f);
	out_color.x = min(1.0f, out_color.x);
	out_color.y = min(1.0f, out_color.y);
	out_color.z = min(1.0f, out_color.z);
	out_color.w = min(1.0f, out_color.w);

	out_color.w = 0.5f;

	return out_color;
}
