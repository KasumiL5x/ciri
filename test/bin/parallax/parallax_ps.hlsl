Texture2D DiffuseTexture : register(t0);
SamplerState DiffuseSampler : register(s0);
Texture2D NormalTexture : register(t1);
SamplerState NormalSampler : register(s1);
Texture2D ParallaxTexture : register(t2);
SamplerState ParallaxSampler : register(s2);

struct Input {
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
static float3 LightDirection = float3(-0.641413, -0.420927, -0.641413);
static float3 LightColor = float3(1.0, 1.0, 1.0);
static float LightIntensity = 1.0;
static float LightPower = 32.0;

static float ParallaxHeight = 0.04;
static float MinLayers = 15.0;
static float MaxLayers = 30.0;

float2 SteepParallaxMapping( in float2 texcoords, in float3 viewdir, out float parallaxHeight ) {
	// number of depth layers
	float numLayers = lerp(MaxLayers, MinLayers, abs(dot(float3(0.0, 0.0, 1.0), viewdir)));
	// calculate size of each layer
	float layerDepth = 1.0 / numLayers;
	// depth of current layer
	float currentLayerDepth = 0.0;
	// amount to shift the texture coordinates by per layer (from vector P)
	float2 P = viewdir.xy / viewdir.z * ParallaxHeight;
	float2 deltaTexcoords = P / numLayers;

	// get initial values
	float2 currentTexcoords = texcoords;
	float currentDepthMapValue = ParallaxTexture.Sample(ParallaxSampler, currentTexcoords).r;

	while( currentLayerDepth < currentDepthMapValue ) {
		// shift texture coordinates along direction of P
		currentTexcoords -= deltaTexcoords;
		// get depth map value at current texture coordinates (use LOD 0 as HLSL can't unroll variable loops)
		currentDepthMapValue = ParallaxTexture.SampleLevel(ParallaxSampler, currentTexcoords, 0).r;
		// get depth of next layer
		currentLayerDepth += layerDepth;
	}

	// get texture coordinates before collision (reverse operations)
	float2 prevTexcoords = currentTexcoords + deltaTexcoords;

	// get depth before and after collision for lerping
	float afterDepth = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = ParallaxTexture.Sample(ParallaxSampler, prevTexcoords).r - currentLayerDepth + layerDepth;

	// interpolation of texture coordinates
	float weight = afterDepth / (afterDepth - beforeDepth);
	float2 finalTexcoords = prevTexcoords * weight + currentTexcoords * (1.0 - weight);

	// output actual height
	parallaxHeight = currentLayerDepth;

	return finalTexcoords;
}

float ParallaxShadow( float3 L, float2 texcoords, float initialHeight ) {
	float shadowMultiplier = 1.0;

	// calculate lighting only for surfaces oriented to the light source
	if( dot(float3(0.0, 0.0, 1.0), L) > 0.0 ) {
		// calculate initial parameters
		float numSamplesUnderSurface = 0.0;
		shadowMultiplier = 0.0;
		float numLayers = lerp(MaxLayers, MinLayers, abs(dot(float3(0.0, 0.0, 1.0), L)));
		float layerHeight = initialHeight / numLayers;
		float2 texStep = ParallaxHeight * L.xy / L.z / numLayers;

		// current parameters
		float currentLayerHeight = initialHeight - layerHeight;
		float2 currentTexcoords = texcoords + texStep;
		float heightFromTexture = ParallaxTexture.Sample(ParallaxSampler, currentTexcoords).r;
		int stepIndex = 1;

		// while point of below depth 0.0
		while( currentLayerHeight > 0.0 ) {
			// if point is under surface
			if( heightFromTexture < currentLayerHeight ) {
				// calculate partial shadowing factor
				numSamplesUnderSurface += 1;
				float newShadowMultiplier = (currentLayerHeight - heightFromTexture) * (1.0 - stepIndex / numLayers);
				shadowMultiplier = max(shadowMultiplier, newShadowMultiplier);
			}

			// offset to the next layer
			stepIndex += 1;
			currentLayerHeight -= layerHeight;
			currentTexcoords += texStep;
			heightFromTexture = ParallaxTexture.SampleLevel(ParallaxSampler, currentTexcoords, 0).r;
		}

		// shadow factor should be 1 if there were no points under the surface
		if( numSamplesUnderSurface < 1 ) {
			shadowMultiplier = 1.0;
		} else {
			shadowMultiplier = 1.0 - shadowMultiplier;
		}
	}
	return shadowMultiplier;
}

float4 main( Input input ) :SV_TARGET {
	float3 ViewDir = normalize(input.tangentCamPos - input.tangentFragPos);
	float2 TexCoords = input.texcoord;
	float selfShadowModifier = 1.0;
	if( true ) {
		float parallaxHeight = 0.0;
		TexCoords = SteepParallaxMapping(input.texcoord, ViewDir, parallaxHeight);
		selfShadowModifier = ParallaxShadow(normalize(input.tangentLightPos - input.tangentFragPos), TexCoords, parallaxHeight - 0.05);
	}

	if( TexCoords.x > 1.0 || TexCoords.y > 1.0 || TexCoords.x < 0.0 || TexCoords.y < 0.0 ) {
		discard;
	}

	float3 Normal = NormalTexture.Sample(NormalSampler, TexCoords).rgb;
	Normal = normalize(Normal * 2.0 - 1.0);

	float3 Color = DiffuseTexture.Sample(DiffuseSampler, TexCoords).rgb;
	float3 Ambient = 0.1 * Color;
	float3 LightDir = normalize(input.tangentLightPos - input.tangentFragPos);
	float Diff = max(dot(LightDir, Normal), 0.0);
	float3 Diffuse = Diff * Color;
	float3 ReflectDir = reflect(-LightDir, Normal);
	float3 HalfDir = normalize(LightDir + ViewDir);
	float Spec = pow(max(dot(Normal, HalfDir), 0.0), 64.0);
	float3 Specular = float3(0.5, 0.5, 0.5) * Spec;

	return float4(Ambient + Diffuse * selfShadowModifier + Specular, 1.0);
}
