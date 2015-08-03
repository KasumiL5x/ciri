struct Input {
	float4 position : SV_POSITION;
	float3 color : COLOR0;
};

float4 main( Input input ) : SV_Target
{
	return float4(input.color, 1.0f);
}
