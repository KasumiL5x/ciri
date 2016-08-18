#version 420

layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_normal;
layout(location=2) in vec4 in_tangent;
layout(location=3) in vec2 in_texcoord;

layout(std140) uniform BasicConstants {
	mat4 world;
	mat4 xform;
	mat4 lightViewProj;
	vec3 campos;
};

out vec3 vo_wpos;
out vec3 vo_wnrm;
out vec3 vo_campos;
out vec4 vo_wposLightSpace;

void main() {
	gl_Position = xform * vec4(in_position, 1.0);
	vo_wpos = (world * vec4(in_position, 1.0)).xyz;
	vo_wnrm = (world * vec4(in_normal, 0.0)).xyz;
	vo_campos = campos;
	vo_wposLightSpace = lightViewProj * vec4(vo_wpos, 1.0);
}
