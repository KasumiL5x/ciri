cbuffer RefractVertexConstants : register(b0) {
	float4x4 world;
	float4x4 xform;
	float3 campos;
};

struct Output {
	float4 hpos : SV_POSITION;
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float2 texcoord : TEXCOORD0;
	// float3 tangent : TEXCOORD1;
	// float3 bitangent : TEXCOORD2;
	float3 viewdir : TEXCOORD3;
};

Output main( float3 Pos : POSITION, float3 Nrm : NORMAL, float4 Tan : TANGENT, float2 Tex : TEXCOORD ) {
	Output OUT;
	OUT.hpos = mul(xform, float4(Pos, 1.0));
	OUT.position = mul(world, float4(Pos, 1.0)).xyz;
	OUT.normal = mul(world, float4(Nrm, 0.0)).xyz;
	OUT.texcoord = Tex;
	// OUT.tangent = mul(world, float4(Tan.xyz, 0.0)).xyz;
	// float3 bt = cross(Nrm, Tan.xyz) * Tan.w;
	// OUT.bitangent = mul(world, float4(bt, 0.0)).xyz;
	OUT.viewdir = (campos - OUT.position);
	return OUT;
}
