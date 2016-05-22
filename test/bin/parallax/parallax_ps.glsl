#version 420

layout(binding=0) uniform sampler2D DiffuseTexture;
layout(binding=1) uniform sampler2D NormalTexture;
layout(binding=2) uniform sampler2D ParallaxTexture;

in vec3 vo_position;
in vec3 vo_normal;
in vec3 vo_viewdir;
in vec2 vo_texcoord;
in mat3 vo_tbn;

in vec3 vo_tangentLightPos;
in vec3 vo_tangentCamPos;
in vec3 vo_tangentFragPos;

out vec4 out_color;

vec3 LightPosition = vec3(0.0, 15.0, 0.0);
vec3 LightDirection = vec3(-0.641413, -0.420927, -0.641413);
vec3 LightColor = vec3(1.0, 1.0, 1.0);
float LightIntensity = 1.0;
float LightPower = 32.0;

float ParallaxHeight = 0.04;
float MinLayers = 15.0;
float MaxLayers = 30.0;

vec2 SteepParallaxMapping( in vec2 texcoords, in vec3 viewdir, out float parallaxHeight ) {
	// number of depth layers
	float numLayers = mix(MaxLayers, MinLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewdir)));
	// calculate size of each layer
	float layerDepth = 1.0 / numLayers;
	// depth of current layer
	float currentLayerDepth = 0.0;
	// amount to shift the texture coordinates by per layer (from vector P)
	vec2 P = viewdir.xy / viewdir.z * ParallaxHeight;
	vec2 deltaTexcoords = P / numLayers;

	// get initial values
	vec2 currentTexcoords = texcoords;
	float currentDepthMapValue = texture(ParallaxTexture, currentTexcoords).r;

	while( currentLayerDepth < currentDepthMapValue ) {
		// shift texture coordinates along direction of P
		currentTexcoords -= deltaTexcoords;
		// get depth map value at current texture coordinates
		currentDepthMapValue = texture(ParallaxTexture, currentTexcoords).r;
		// get depth of next layer
		currentLayerDepth += layerDepth;
	}

	// get texture coordinates before collision (reverse operations)
	vec2 prevTexcoords = currentTexcoords + deltaTexcoords;

	// get depth before and after collision for lerping
	float afterDepth = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = texture(ParallaxTexture, prevTexcoords).r - currentLayerDepth + layerDepth;

	// interpolation of texture coordinates
	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 finalTexcoords = prevTexcoords * weight + currentTexcoords * (1.0 - weight);

	// output actual height
	parallaxHeight = currentLayerDepth;

	return finalTexcoords;
}

float ParallaxShadow( vec3 L, vec2 texcoords, float initialHeight ) {
	float shadowMultiplier = 1.0;

	// calculate lighting only for surfaces oriented to the light source
	if( dot(vec3(0.0, 0.0, 1.0), L) > 0.0 ) {
		// calculate initial parameters
		float numSamplesUnderSurface = 0.0;
		shadowMultiplier = 0.0;
		float numLayers = mix(MaxLayers, MinLayers, abs(dot(vec3(0.0, 0.0, 1.0), L)));
		float layerHeight = initialHeight / numLayers;
		vec2 texStep = ParallaxHeight * L.xy / L.z / numLayers;

		// current parameters
		float currentLayerHeight = initialHeight - layerHeight;
		vec2 currentTexcoords = texcoords + texStep;
		float heightFromTexture = texture(ParallaxTexture, currentTexcoords).r;
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
			heightFromTexture = texture(ParallaxTexture, currentTexcoords).r;
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

void main() {
	vec3 ViewDir = normalize(vo_tangentCamPos - vo_tangentFragPos);
	vec2 TexCoords = vo_texcoord;
	float selfShadowModifier = 1.0;
	if( true ) {
		float parallaxHeight = 0.0;
		TexCoords = SteepParallaxMapping(vo_texcoord, ViewDir, parallaxHeight);
		selfShadowModifier = ParallaxShadow(normalize(vo_tangentLightPos - vo_tangentFragPos), TexCoords, parallaxHeight - 0.05);
	}

	if( TexCoords.x > 1.0 || TexCoords.y > 1.0 || TexCoords.x < 0.0 || TexCoords.y < 0.0 ) {
		discard;
	}

	vec3 Normal = texture(NormalTexture, TexCoords).rgb;
	Normal = normalize(Normal * 2.0 - 1.0);

	vec3 Color = texture(DiffuseTexture, TexCoords).rgb;
	vec3 Ambient = 0.1 * Color;
	vec3 LightDir = normalize(vo_tangentLightPos - vo_tangentFragPos);
	float Diff = max(dot(LightDir, Normal), 0.0);
	vec3 Diffuse = Diff * Color;
	vec3 ReflectDir = reflect(-LightDir, Normal);
	vec3 HalfDir = normalize(LightDir + ViewDir);
	float Spec = pow(max(dot(Normal, HalfDir), 0.0), 64.0);
	vec3 Specular = vec3(0.5) * Spec;
	out_color = vec4(Ambient + Diffuse * selfShadowModifier + Specular, 1.0);
}
