struct Input {
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};

float4 main( Input input ) : SV_Target
{
	return float4(input.Tex, 0.0f, 1.0f);
}
