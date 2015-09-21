#version 420

uniform sampler2D SpriteTexture;

out vec4 out_color;

in vec2 go_texcoord;

void main() {
	out_color = vec4(texture(SpriteTexture, go_texcoord).xyz, 1.0f);
	// out_color = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}