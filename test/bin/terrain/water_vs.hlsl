cbuffer WaterConstants : register(b0) {
	float4x4 world;
	float4x4 xform;
	float4x4 reflectedViewProj;
	float3 campos;
	float time;
};

struct Output {
	float4 hpos          : SV_POSITION;
	float3 position      : TEXCOORD1;
	float3 normal        : NORMAL0;
	float3 camPos        : TEXCOORD2;
	float2 bumpTexcoords : TEXCOORD3;
	float3 cubeTexcoords : TEXCOORD4;
	float waveHeight     : TEXCOORD5;
	float4 reflectionTexcoords : TEXCOORD6;
	float4 refractionTexcoords : TEXCOORD7;
};

// -*- water variables -*-
static float waveLength = 0.5f;
static float waveHeight = 0.3f;
static float windForce = 0.008f;
static float3 windDirection = normalize(float3(0.0f, 0.0f, 1.0f));

Output main( float3 in_position : POSITION, float3 in_normal : NORMAL, float4 in_tangent : TANGENT, float2 in_texcoord : TEXCOORD ) {
	Output OUT;

	OUT.hpos = mul(xform, float4(in_position, 1.0f));

	// world position
	OUT.position = mul(world, float4(in_position, 1.0f)).xyz;

	// world normal
	OUT.normal = mul(world, float4(in_normal, 0.0f)).xyz;

	// camera position
	OUT.camPos = campos;

	// bumpmap perturbed texcoords
	float3 perpDir = cross(windDirection, float3(0.0f, 1.0f, 0.0f));
	float ydot = dot(in_texcoord, windDirection.xz);
	float xdot = dot(in_texcoord, perpDir.xz);
	float2 moveVector = float2(xdot, ydot);
	moveVector += time * windForce;
	OUT.bumpTexcoords = moveVector / waveLength;

	// cubemap texcoords
	float3 V = normalize(OUT.position - campos);
	float3 RV = normalize(reflect(V, OUT.normal));
	OUT.cubeTexcoords = float3(RV.x, -RV.y, RV.z);

	// calculate reflection texcoords
	float4x4 reflectionMatrix = mul(reflectedViewProj, world);
	OUT.reflectionTexcoords = mul(reflectionMatrix, float4(in_position, 1.0f));

	// calculate refraction texcoords
	OUT.refractionTexcoords = mul(xform, float4(in_position, 1.0f));


	// water variables
	OUT.waveHeight = waveHeight;

	return OUT;
}
