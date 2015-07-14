struct Output {
	float4 Pos : SV_POSITION;
	float3 Col : COLOR0;
};

Output main( float3 Pos : POSITION, float3 Col : COLOR )
{
	Output OUT;
	OUT.Pos = float4(Pos, 1.0f);
	OUT.Col = Col;
	return OUT;
}
