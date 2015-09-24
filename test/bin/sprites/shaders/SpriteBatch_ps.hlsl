Texture2D SpriteTexture : register(t0);
SamplerState SpriteSampler : register(s0);

struct Input {
	float4 hposition : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

float4 main( Input input ) : SV_Target {
	return SpriteTexture.Sample(SpriteSampler, input.texcoord);
	// return float4(1.0f, 0.0f, 1.0f, 1.0f);
}
