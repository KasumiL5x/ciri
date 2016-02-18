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

// in vec3 vo_tangent;
// in vec3 vo_bitangent;

out vec4 out_color;

vec3 LightPosition = vec3(0.0, 15.0, 0.0);
vec3 LightDirection = vec3(-0.641413, -0.420927, -0.641413);
vec3 LightColor = vec3(1.0, 1.0, 1.0);
float LightIntensity = 1.0;
float LightPower = 32.0;

float attenuate( vec3 p, vec3 l, float range )
{
	float dist = length(p - l);
	return smoothstep(range, 0.0f, dist);
}

vec3 lambert( vec3 L, vec3 N, vec3 lightColor, float lightIntensity )
{
	float diffuseLight = max(dot(L, N), 0.0f);
	return (diffuseLight * lightColor) * lightIntensity;
}

vec3 phong( vec3 L, vec3 N, vec3 V, vec3 lightColor, float lightIntensity, float specularPower )
{
	float nDotL = max(dot(N, L), 0.0f);
	vec3 RL = reflect(-L, N);
	float rDotV = max(dot(RL, V), 0.0f);
	vec3 specular = vec3(0.0f);
	if( specularPower > 0.0f )
	{
		specular = (lightColor * pow(rDotV, specularPower));
	}
	vec3 diffuse = (lightColor * nDotL);
	return (diffuse + specular) * lightIntensity;
}

vec2 ParallaxMapping( vec2 texcoords, vec3 viewdir ) {
	// standard parallax
	float height_scale = 0.04;
	float height = texture(ParallaxTexture, texcoords).r;
	return texcoords - (viewdir.xy / viewdir.z * (height * height_scale));
}

vec2 SteepParallaxMapping( vec2 texcoords, vec3 viewdir ) {
	float height_scale = 0.04;

	// number of depth layers
	const float minLayers = 10.0;
	const float maxLayers = 20.0;
	float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewdir)));
	// calculate size of each layer
	float layerDepth = 1.0 / numLayers;
	// depth of current layer
	float currentLayerDepth = 0.0;
	// amount to shift the texture coordinates by per layer (from vector P)
	vec2 P = viewdir.xy / viewdir.z * height_scale;
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

	return currentTexcoords;
}

void main() {
	vec3 ViewDir = normalize(vo_tangentCamPos - vo_tangentFragPos);
	vec2 TexCoords = vo_texcoord;
	if( true ) {
		TexCoords = SteepParallaxMapping(vo_texcoord, ViewDir);
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
	out_color = vec4(Ambient + Diffuse + Specular, 1.0);
	return;


	// light vector
	vec3 L = normalize(LightPosition - vo_position);
	// vec3 L = normalize(-LightDirection);

	// view direction
	vec3 V = normalize(vo_viewdir);

	// texcoords
	vec2 UV = vo_texcoord;
	if( true ) {
		UV = ParallaxMapping(vo_texcoord, V);
	}

	// normal
	vec3 N = normalize(vo_normal);
	if( true ) {
		N = normalize(texture(NormalTexture, UV).xyz * 2.0 - 1.0);
		N = normalize(vo_tbn * N);
	}

	float atten = 1.0;//attenuate(vo_position, LightPosition, 10.0);
	vec3 lighting = phong(L, N, V, LightColor, LightIntensity, LightPower) * atten;
	// vec3 lighting = lambert(L, N, LightColor, intensity) * atten;

	vec3 albedo = texture(DiffuseTexture, UV).xyz;

	out_color = vec4(albedo * lighting, 1.0);

	// out_color = texture(ParallaxTexture, UV);
}

// old camera/eye space stuff
//
////// INPUTS
//
// in vec3 vo_position_ws;
// in vec3 vo_eye_cs;
// in vec3 vo_lightdir_cs;
// in vec3 vo_normal_cs;
// in vec2 vo_texcoord;
//
////// SHADER
//
// // light stuff
// vec3 LightColor = vec3(1.0, 1.0, 1.0);
// float LightPower = 50.0;
// // distance to light
// float distance = length(vo_position_ws - LightPosition);
// // normal of computed fragment in camera space
// vec3 N = normalize(vo_normal_cs);
// // direction of light (from fragment to the light)
// vec3 L = normalize(vo_lightdir_cs);
// // cos of angle between normal and light dir
// float cosTheta = clamp(dot(N, L), 0.0, 1.0);
// // eye vector (towards camera)
// vec3 E = normalize(vo_eye_cs);
// // direction triangle reflects light
// vec3 R = reflect(-L, N);
// // cos of angle between eye vector and reflect vector
// float cosAlpha = clamp(dot(E, R), 0.0, 1.0);
// // diffuse result
// vec3 diffuse = vec3(1.0, 1.0, 1.0) * LightColor * LightPower * cosTheta / (distance * distance);
// // specular result
// vec3 specular = vec3(1.0, 1.0, 1.0) * LightColor * LightPower * pow(cosAlpha, 5.0) / (distance * distance);
// // final result
// out_color = vec4(diffuse + specular, 1.0);
