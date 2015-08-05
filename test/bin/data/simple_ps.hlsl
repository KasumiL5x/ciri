Texture2D TestTexture : register(t0);
SamplerState TestSampler : register(s0);

struct Input {
	float4 Pos : SV_POSITION;
	float3 position : TEXCOORD1;
	float3 normal : NORMAL0;
	float2 Tex : TEXCOORD0;
};

float3 AmbientLightColor = float3(0.05333332f, 0.09882354f, 0.1819608f);
float3 LightDirection = float3(-0.5265408f, -0.5735765f,-0.6275069f);
float3 LightColor = float3(1.0f, 0.9607844f, 0.8078432f);

float3 lambert( float3 L, float3 N, float3 lightColor, float lightIntensity ) {
	return (max(dot(L, N), 0.0f) * lightColor) * lightIntensity;
}

float3 phong( float3 L, float3 N, float3 lightColor, float lightIntensity, float specularPower, float3 worldPos, float3 cameraPos ) {
	float nDotL = max(dot(L, N), 0.0f);
	float3 RL = normalize(2.0f * N * nDotL - L);
	float3 V = normalize(cameraPos - worldPos);
	float rDotV = max(dot(RL, V), 0.0f);
	float3 specular = float3(0.0f, 0.0f, 0.0f);
	if( specularPower > 0.0f ) {
		specular = lightColor * pow(rDotV, specularPower);
	}
	return (lightColor * nDotL + specular) * lightIntensity;
}

float4 main( Input input ) : SV_Target {
	// lighting
	float3 L = -LightDirection;
	float3 N = normalize(input.normal);
	float3 lighting = AmbientLightColor + lambert(L, N, LightColor, 1.0f);
	// texture sample
	float3 textureSample = TestTexture.Sample(TestSampler, input.Tex).rgb;
	// final
	return float4(textureSample * lighting, 1.0f);
}
