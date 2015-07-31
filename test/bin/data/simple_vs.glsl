#version 330

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texcoord;

layout (std140) uniform SimpleConstants {
	mat4 xform;
};

out vec2 vo_texcoord;

void main() {
	gl_Position = xform * vec4(in_position, 1.0f);
	vo_texcoord = in_texcoord;
}
