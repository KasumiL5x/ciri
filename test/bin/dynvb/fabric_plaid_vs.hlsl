cbuffer Constants : register(b0) {
	float4x4 world;
	float4x4 xform;
	float3 campos;
};

struct Output {
	float4 Pos : SV_POSITION;
	float3 position : TEXCOORD1;
	float3 normal : NORMAL0;
	float2 Tex : TEXCOORD0;
	float3 CamPos : TEXCOORD2;
};

Output main( float3 Pos : POSITION, float3 Nrm : NORMAL, float4 Tan : TANGENT, float2 Tex : TEXCOORD ) {
	Output OUT;
	OUT.Pos = mul(xform, float4(Pos, 1.0f));
	OUT.position = mul(world, float4(Pos, 1.0f)).xyz;
	OUT.normal = mul(world, float4(Nrm, 0.0f)).xyz;
	OUT.Tex = Tex;
	OUT.CamPos = campos;
	return OUT;
}
