#version 330

in vec2 vo_texcoord;

out vec4 out_color;

void main() {
	out_color = vec4(vo_texcoord, 0.0f, 1.0f);
}
