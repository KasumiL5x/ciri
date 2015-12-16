TextureCube CubemapTexture : register(t0);
SamplerState CubemapSampler : register(s0);

struct Input {
	float4 Pos : SV_POSITION;
	float3 position : TEXCOORD1;
	float3 normal : NORMAL0;
	float2 Tex : TEXCOORD0;
	float3 ViewDir : TEXCOORD2;
};

static float3 AmbientLightColor = float3(0.05333332f, 0.09882354f, 0.1819608f);
static float3 LightDirection = float3(-0.5265408f, -0.5735765f,-0.6275069f);
static float3 LightColor = float3(1.0f, 0.9607844f, 0.8078432f);

static float4 DiffuseColor = float4(1.0, 1.0, 1.0, 1.0);
static float4 AmbientColor = float4(0.5, 0.5, 0.5, 1.0);



float3 lambert( float3 L, float3 N, float3 lightColor, float lightIntensity ) {
	return (max(dot(L, N), 0.0f) * lightColor) * lightIntensity;
}

float4 main( Input input ) : SV_Target {
	float3 L = normalize(-LightDirection);
	float3 N = normalize(input.normal);

	float diffuseLight = max(dot(L, N), 0.0);

	float AIR = 1.00029;
	float ICE = 1.31;
	float RIDX = AIR / ICE;

	float3 V = normalize(input.ViewDir);
	float3 Refract = refract(V, N, RIDX);
	float3 refractColor = CubemapTexture.Sample(CubemapSampler, Refract);

	return float4(AmbientColor * float4(refractColor, 1.0) + DiffuseColor * diffuseLight);
}
