cbuffer AxisConstants : register(b0) {
	float4x4 xform;
};

struct Output {
	float4 position : SV_POSITION;
	float3 color : COLOR0;
};

Output main( float3 position : POSITION, float3 color : COLOR ) {
	Output OUT;
	OUT.position = mul(xform, float4(position, 1.0f));
	OUT.color = color;
	return OUT;
}
