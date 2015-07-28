Texture2D TestTexture : register(t0);
SamplerState TestSampler : register(s0);

struct Input {
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};

float4 main( Input input ) : SV_Target
{
	float4 textureSample = TestTexture.Sample(TestSampler, input.Tex);
	return float4(textureSample.rgb, 1.0f);
	// return float4(input.Tex, 0.0f, 1.0f);
}
