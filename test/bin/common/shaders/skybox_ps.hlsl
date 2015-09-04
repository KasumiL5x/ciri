TextureCube SkyboxTexture : register(t0);
SamplerState SkyboxSampler : register(s0);

struct Input {
	float4 hpos : SV_POSITION;
	float3 texcoord : TEXCOORD0;
};

float4 main( Input input ) : SV_Target {
	float3 texSample = SkyboxTexture.Sample(SkyboxSampler, input.texcoord).xyz;
	return float4(texSample, 1.0f);
}
