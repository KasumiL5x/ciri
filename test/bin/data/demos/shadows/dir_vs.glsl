#version 420

layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_normal;
layout(location=2) in vec4 in_tangent;
layout(location=3) in vec2 in_texcoord;

layout(std140) uniform DirectionalConstants {
	mat4 world;
	mat4 xform;
	vec3 campos;
	float LightIntensity;
	vec3 LightDirection;
	int CastShadows;
	vec3 LightColor;
	float pad0;
	mat4 lightViewProj;
};

out vec3 vo_wpos;
out vec3 vo_wnrm;
out vec3 vo_campos;
out vec3 vo_LightDirection;
out vec3 vo_LightColor;
out float vo_LightIntensity;
flat out int vo_CastShadows;
out vec4 vo_wposLightSpace;

void main() {
	gl_Position = xform * vec4(in_position, 1.0);
	vo_wpos = (world * vec4(in_position, 1.0)).xyz;
	vo_wnrm = (world * vec4(in_normal, 0.0)).xyz;
	vo_campos = campos;
	vo_LightDirection = LightDirection;
	vo_LightColor = LightColor;
	vo_LightIntensity = LightIntensity;
	vo_CastShadows = CastShadows;
	vo_wposLightSpace = lightViewProj * vec4(vo_wpos, 1.0);
}
