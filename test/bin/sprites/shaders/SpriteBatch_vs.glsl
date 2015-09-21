#version 420

layout (location = 0) in vec2 in_position;
layout (location = 1) in vec2 in_scale;

out vec2 vo_scale;

void main() {
	gl_Position = vec4(in_position, 0.0f, 1.0f);
	vo_scale = in_scale;
}
