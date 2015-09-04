cbuffer SkyboxConstants : register(b0) {
	float4x4 view;
	float4x4 proj;
};

struct Output {
	float4 hpos : SV_POSITION;
	float3 texcoord : TEXCOORD0;
};

Output main( float3 Pos : POSITION, float3 Nrm : NORMAL, float4 Tan : TANGENT, float2 Tex : TEXCOORD ) {
	Output OUT;
	OUT.hpos = float4(Pos, 1.0f);
	float4x4 inverseProjection = proj;
	float3x3 view33 = (float3x3)view;
	view33 = transpose(view33);
	float3 unprojected = mul(inverseProjection, float4(Pos, 1.0f)).xyz;
	OUT.texcoord = mul(view33, unprojected);
	OUT.texcoord.y = -OUT.texcoord.y;
	return OUT;
}
