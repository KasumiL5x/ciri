struct VSOut {
	float4 hposition : SV_POSITION;
	float3 wpos : POSITION;
	float3 wnrm : NORMAL;
	float3 campos : TEXCOORD0;
};

static float3 AmbientLightColor = float3(0.05333332, 0.09882354, 0.1819608);
static float3 DirLightDirection = float3(-0.5265408, -0.5735765,-0.6275069);
static float3 DirLightDiffuseColor = float3(1.0, 0.9607844, 0.8078432);

float3 phong( float3 L, float3 N, float3 lightColor, float lightIntensity, float specularPower, float3 worldPos, float3 camPos ) {
	float nDotL = max(dot(N, L), 0.0);
	float3 RL = normalize(2.0 * N * nDotL - L);
	float3 V = normalize(camPos - worldPos);
	float rDotV = max(dot(RL, V), 0.0);
	float3 specular = float3(0.0, 0.0, 0.0);
	if( specularPower > 0.0 ) {
		specular = lightColor * pow(rDotV, specularPower);
	}
	float3 diffuse = lightColor * nDotL;
	return (diffuse + specular) * lightIntensity;
}

float4 main( VSOut input ) : SV_Target {
	float3 L = -DirLightDirection;
	float3 N = normalize(input.wnrm);
	float3 lighting = AmbientLightColor + phong(L, N, DirLightDiffuseColor, 1.0, 128.0, input.wpos, input.campos);
	return float4(lighting, 1.0);
}
