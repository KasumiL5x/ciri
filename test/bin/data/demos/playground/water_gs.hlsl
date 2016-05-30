struct VSOutput {
	float4 hposition : SV_POSITION;
	float3 normal : NORMAL0;
	float2 texcoord : TEXCOORD0;
	float4x4 World : TEXCOORD1;
	float3x3 InvWorld : TANGENT0;
	float3 CamPos : NORMAL1;
	float4x4 XFORM : BINORMAL0;
};


struct GSOutput {
	float4 hposition : SV_POSITION;
	float3 normal : NORMAL;
	float2 texcoord : TEXCOORD0;
	float3 diffuse : TEXCOORD1;
	float3 specular : TEXCOORD2;
};

[maxvertexcount(3)]
void main( triangle VSOutput input[3], inout TriangleStream<GSOutput> OutputStream ) {
	GSOutput output = (GSOutput)0;

	float3 v0 = input[0].hposition.xyz;
	float3 v1 = input[1].hposition.xyz;
	float3 v2 = input[2].hposition.xyz;

	float3 centerPos = (v0 + v1 + v2) / 3.0;

	float3 vn = normalize(cross(v1 - v0, v2 - v0));

	float3 normalDirection = normalize(mul(float4(vn, 0.0), (float3x3)input[0].InvWorld).xyz);
	float3 viewDirection = normalize(input[0].CamPos - mul(input[0].World, float4(centerPos, 0.0)).xyz);
	float3 lightDirection = normalize(float3(100.0, 300.0, 42.0));
	float attenuation = 1.0;

	float3 Color = float3(0.56, 0.85, 0.83);//float3(0.13, 0.7, 0.66);//float3(0.0, 0.412, 0.58);
	float3 LightColor = float3(1.0, 1.0, 1.0);
	float3 SpecColor = float3(1.0, 1.0, 1.0);
	float Shininess = 64.0;
	float3 ambientLighting = float3(0.1, 0.1, 0.1) * Color;

	float3 diffuseReflection = attenuation * LightColor * Color * max(0.0, dot(normalDirection, lightDirection));

	float3 specularReflection;
	if( dot(normalDirection, lightDirection) < 0.0 ) {
		specularReflection = float3(0.0, 0.0, 0.0);
	} else {
		specularReflection = attenuation * LightColor * SpecColor * pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)), Shininess);
	}

	output.hposition = mul(input[0].XFORM, input[0].hposition);
	output.normal = vn;
	output.texcoord = input[0].texcoord;
	output.diffuse = ambientLighting + diffuseReflection;
	output.specular = specularReflection;
	OutputStream.Append(output);

	output.hposition = mul(input[0].XFORM, input[1].hposition);
	output.normal = vn;
	output.texcoord = input[1].texcoord;
	output.diffuse = ambientLighting + diffuseReflection;
	output.specular = specularReflection;
	OutputStream.Append(output);

	output.hposition = mul(input[0].XFORM, input[2].hposition);
	output.normal = vn;
	output.texcoord = input[2].texcoord;
	output.diffuse = ambientLighting + diffuseReflection;
	output.specular = specularReflection;
	OutputStream.Append(output);

	OutputStream.RestartStrip();
}
