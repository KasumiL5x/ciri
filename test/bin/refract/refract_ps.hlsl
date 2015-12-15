struct Input {
	float4 Pos : SV_POSITION;
	float3 position : TEXCOORD1;
	float3 normal : NORMAL0;
	float2 Tex : TEXCOORD0;
};

static float3 AmbientLightColor = float3(0.05333332f, 0.09882354f, 0.1819608f);
static float3 LightDirection = float3(-0.5265408f, -0.5735765f,-0.6275069f);
static float3 LightColor = float3(1.0f, 0.9607844f, 0.8078432f);

float3 lambert( float3 L, float3 N, float3 lightColor, float lightIntensity ) {
	return (max(dot(L, N), 0.0f) * lightColor) * lightIntensity;
}

float4 main( Input input ) : SV_Target {
	// lighting
	float3 L = -LightDirection;
	float3 N = normalize(input.normal);
	float3 lighting = AmbientLightColor + lambert(L, N, LightColor, 1.0f);


	float Air = 1.0;
	float Glass = 1.51714;
	float R0 = ((Air - Glass) * (Air - Glass)) / ((Air + Glass) * (Air + Glass));
	float fresnel = R0 + (1.0 - R0) * pow((1.0 - dot(-L, N)), 5.0);
	float3 color0 = float3(0.0f, 0.0f, 0.0f);
	float3 color1 = float3(1.0f, 1.0f, 1.0f);
	return float4(lerp(color0, color1, fresnel), 1.0f);

	// final
	//return float4(lighting, 1.0f);
}