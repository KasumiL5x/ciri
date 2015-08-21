#version 330

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texcoord;

layout (std140) uniform Constants {
	mat4 world;
	mat4 xform;
	vec3 campos;
};

out vec3 vo_position;
out vec3 vo_normal;
out vec2 vo_texcoord;
out vec3 vo_camPos;

void main() {
	gl_Position = xform * vec4(in_position, 1.0f);
	vo_position = (world * vec4(in_position, 1.0f)).xyz;
	vo_normal = (world * vec4(in_normal, 0.0f)).xyz;
	vo_texcoord = in_texcoord;
	vo_camPos = campos;
}
