#version 330

in vec3 in_position;
in vec3 in_color;

out vec3 vo_color;

void main() {
	gl_Position = vec4(in_position, 1.0f);
	vo_color = in_color;
}
