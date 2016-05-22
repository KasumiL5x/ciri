cbuffer ParallaxVertexConstants : register(b0) {
	float4x4 world;
	float4x4 xform;
	float3 campos;
};

struct Output {
	float4 hposition : SV_POSITION;
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 viewdir : TEXCOORD1;
	float2 texcoord : TEXCOORD;
	float3x3 tbn : TEXCOORD2;
	float3 tangentLightPos : TANGENT0;
	float3 tangentCamPos : TANGENT1;
	float3 tangentFragPos : TANGENT2;
};

static float3 LightPosition = float3(0.0, 15.0, 0.0);

Output main( float3 in_position:POSITION, float3 in_normal:NORMAL, float4 in_tangent:TANGENT, float2 in_texcoord:TEXCOORD ) {
	Output OUT;

	OUT.hposition = mul(xform, float4(in_position, 1.0));

	OUT.position = mul(world, float4(in_position, 1.0)).xyz;
	OUT.normal = mul(world, float4(in_normal, 0.0)).xyz;
	OUT.viewdir = (campos - OUT.position);

	float3 T = normalize(mul(world, float4(in_tangent.xyz, 0.0)).xyz);
	float3 N = normalize(mul(world, float4(in_normal, 0.0)).xyz);
	float3 BT = cross(N, T) * in_tangent.w;
	float3 B = normalize(mul(world, float4(BT, 0.0)).xyz);
	OUT.tbn = float3x3(T, B, N);

	OUT.tangentLightPos = mul(OUT.tbn, LightPosition);
	OUT.tangentCamPos = mul(OUT.tbn, campos);
	OUT.tangentFragPos = mul(OUT.tbn, OUT.position);

	OUT.texcoord = in_texcoord;

	return OUT;
}
