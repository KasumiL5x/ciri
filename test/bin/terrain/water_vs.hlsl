cbuffer WaterConstants : register(b0) {
	float4x4 world;
	float4x4 worldview;
	float4x4 xform;
	float3 campos;
};

struct Output {
	float4 hpos : SV_POSITION;
	float3 wpos : TEXCOORD1;
	float3 nrm : NORMAL0;
	float2 tex : TEXCOORD0;
	float3 campos : TEXCOORD2;
	float3x3 tbn : TEXCOORD3;
};

Output main( float3 Pos : POSITION, float3 Nrm : NORMAL, float4 Tan : TANGENT, float2 Tex : TEXCOORD ) {
	Output OUT;
	OUT.hpos = mul(xform, float4(Pos, 1.0f));
	OUT.wpos = mul(world, float4(Pos, 1.0f)).xyz;
	OUT.nrm = mul(world, float4(Nrm, 0.0f)).xyz;
	OUT.tex = Tex;
	OUT.campos = campos;

	float3 n = normalize(mul(world, float4(Nrm, 0.0f)).xyz);
	float3 t = normalize(mul(world, float4(Tan.xyz, 0.0f)).xyz);
	float3 b = normalize(mul(world, float4(cross(Nrm, Tan.xyz) * Tan.w, 0.0f)).xyz);
	OUT.tbn = float3x3(t, b, n);

	return OUT;
}
