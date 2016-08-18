#version 420

layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_normal;
layout(location=2) in vec4 in_tangent;
layout(location=3) in vec2 in_texcoord;

layout(std140) uniform SpotlightConstants {
	mat4 world;
	mat4 xform;
	vec3 campos;
	float LightCosInner;
	vec3 LightPosition;
	float LightCosOuter;
	vec3 LightDirection;
	float LightIntensity;
	vec3 LightColor;
	float LightRange;
	mat4 lightViewProj;
	int CastShadows;
};

out vec3 vo_wpos;
out vec3 vo_wnrm;
out vec3 vo_campos;
out vec3 vo_LightPosition;
out vec3 vo_LightDirection;
out vec3 vo_LightColor;
out float vo_LightCosInner;
out float vo_LightCosOuter;
out float vo_LightIntensity;
out float vo_LightRange;
flat out int vo_CastShadows;
out vec4 vo_wposLightSpace;

void main() {
	gl_Position = xform * vec4(in_position, 1.0);
	vo_wpos = (world * vec4(in_position, 1.0)).xyz;
	vo_wnrm = (world * vec4(in_normal, 0.0)).xyz;
	vo_campos = campos;
	vo_LightPosition = LightPosition;
	vo_LightDirection = LightDirection;
	vo_LightColor = LightColor;
	vo_LightCosInner = LightCosInner;
	vo_LightCosOuter = LightCosOuter;
	vo_LightIntensity = LightIntensity;
	vo_LightRange = LightRange;
	vo_CastShadows = CastShadows;
	vo_wposLightSpace = lightViewProj * vec4(vo_wpos, 1.0);
}
