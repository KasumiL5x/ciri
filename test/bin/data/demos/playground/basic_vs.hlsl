cbuffer BasicConstants : register(b0) {
	float4x4 world;
	float4x4 xform;
	float3 campos;
};

struct VSOut {
	float4 hposition : SV_POSITION;
	float3 wpos : POSITION;
	float3 wnrm : NORMAL;
	float3 campos : TEXCOORD0;
};

VSOut main( float3 in_position:POSITION, float3 in_normal:NORMAL, float4 in_tangent:TANGENT, float2 in_texcoord:TEXCOORD ) {
	VSOut output;

	output.hposition = mul(xform, float4(in_position, 1.0));
	output.wpos = mul(world, float4(in_position, 1.0)).xyz;
	output.wnrm = mul(world, float4(in_normal, 0.0)).xyz;
	output.campos = campos;

	return output;
}
