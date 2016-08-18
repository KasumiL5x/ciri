#version 440

in vec3 go_normal;
in vec2 go_texcoord;
in vec3 go_diffuse;
in vec3 go_specular;

out vec4 out_color;

void main() {
	out_color = vec4(go_specular + go_diffuse, 1.0);
}
