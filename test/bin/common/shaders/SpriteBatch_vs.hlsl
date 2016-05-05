cbuffer SpriteConstants : register(b0) {
	float4x4 projection;
};

struct Output {
	float4 hposition : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float4 color : COLOR0;
};

Output main( float3 in_position : POSITION, float2 in_texcoord : TEXCOORD, float4 in_color : COLOR0 ) {
	Output OUT;
	OUT.hposition = mul(projection, float4(in_position.xy, 0.0f, 1.0f));
	OUT.hposition.z = in_position.z;
	OUT.texcoord = in_texcoord;
	OUT.color = in_color;
	return OUT;
}
