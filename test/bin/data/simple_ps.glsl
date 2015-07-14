#version 330

in vec3 vo_color;

out vec4 out_color;

void main() {
	out_color = vec4(vo_color, 1.0f);
	//out_color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}
