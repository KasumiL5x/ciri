#version 420

in vec3 vo_color;

out vec4 out_color;

void main() {
	out_color = vec4(vo_color, 1.0f);
}
