cbuffer WaterConstants : register(b0) {
	float4x4 world;
	float4x4 xform;
	float ElapsedTime;
	float3 CamPos;
};

struct VSOutput {
	float4 hposition : SV_POSITION;
	float3 normal : NORMAL0;
	float2 texcoord : TEXCOORD0;
	float4x4 World : TEXCOORD1;
	float3x3 InvWorld : TANGENT0;
	float3 CamPos : NORMAL1;
	float4x4 XFORM : BINORMAL0;
};

static float WaveHeight = 3.5;
static float WaveSpeed = 1.0;
static float WaveLength = 10.5;
static float RandomHeight = 0.5;
static float RandomSpeed = 0.5;

float rand( float3 co ) {
	return frac(sin(dot(co.xyz ,float3(12.9898,78.233,45.5432))) * 43758.5453);
}

float rand2( float3 co ) {
	return frac(sin(dot(co.xyz ,float3(19.9128,75.2,34.5122))) * 12765.5213);
}

VSOutput main( float3 in_position:POSITION, float3 in_normal:NORMAL, float4 in_tangent:TANGENT, float2 in_texcoord:TEXCOORD ) {
	VSOutput OUT;

	float3 v0 = mul(world, float4(in_position, 1.0)).xyz;
	float phase0 = (WaveHeight) * sin((ElapsedTime * WaveSpeed) + (v0.x * WaveLength) + (v0.z * WaveLength) + rand2(v0.xzz));
	float phase1 = (RandomHeight) * sin(cos(rand(v0.xzz) * RandomHeight * cos(ElapsedTime * RandomSpeed * sin(rand(v0.xxz)))));
	v0.y += phase0 + phase1;
	in_position.xyz = mul((float3x3)transpose(world), v0);

	OUT.hposition = float4(in_position, 1.0);
	OUT.normal = in_normal;
	OUT.texcoord = in_texcoord;

	OUT.World = world;
	OUT.InvWorld = (float3x3)transpose(world);

	OUT.CamPos = CamPos;

	OUT.XFORM = xform;

	return OUT;
}
