struct GSOutput {
	float4 hposition : SV_POSITION;
	float3 normal : NORMAL;
	float2 texcoord : TEXCOORD0;
	float3 diffuse : TEXCOORD1;
	float3 specular : TEXCOORD2;
};

float4 main( GSOutput input ) : SV_Target { 
	return float4(input.specular + input.diffuse, 1.0);
}
