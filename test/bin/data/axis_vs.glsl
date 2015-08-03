#version 330

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_color;

layout (std140) uniform AxisConstants {
	mat4 xform;
};

out vec3 vo_color;

void main() {
	gl_Position = xform * vec4(in_position, 1.0f);
	vo_color = in_color;
}
