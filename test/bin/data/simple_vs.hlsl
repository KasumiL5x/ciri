cbuffer SimpleConstants : register(b0) {
	float4x4 xform;
};

struct Output {
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};

Output main( float3 Pos : POSITION, float3 Nrm : NORMAL, float2 Tex : TEXCOORD )
{
	Output OUT;
	OUT.Pos = mul(xform, float4(Pos, 1.0f));
	OUT.Tex = Tex;
	return OUT;
}
