#version 420

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec4 in_tangent;
layout (location = 3) in vec2 in_texcoord;

layout (std140) uniform RefractVertexConstants {
	mat4 world;
	mat4 xform;
	vec3 campos;
};

out vec3 vo_position;
out vec3 vo_normal;
out vec2 vo_texcoord;
out vec3 vo_tangent;
out vec3 vo_bitangent;
out vec3 vo_viewdir;

void main() {
	gl_Position = xform * vec4(in_position, 1.0);
	vo_position = (world * vec4(in_position, 1.0)).xyz;
	vo_normal = (world * vec4(in_normal, 0.0f)).xyz;
	vo_texcoord = in_texcoord;
	vo_tangent = (world * vec4(in_tangent.xyz, 0.0)).xyz;
	vec3 bt = cross(in_normal, in_tangent.xyz) * in_tangent.w;
	vo_bitangent = (world * vec4(bt, 0.0)).xyz;
	vo_viewdir = (campos - vo_position);
}
