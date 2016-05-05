#version 420

uniform sampler2D SpriteTexture;

out vec4 out_color;

in vec2 vo_texcoord;
in vec4 vo_color;

void main() {
	out_color = texture(SpriteTexture, vo_texcoord) * vo_color;
	// out_color = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}
