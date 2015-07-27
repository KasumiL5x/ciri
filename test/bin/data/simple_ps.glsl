#version 330

uniform sampler2D TestTexture;

in vec2 vo_texcoord;

out vec4 out_color;

void main() {
	vec4 textureSample = texture(TestTexture, vo_texcoord);
	out_color = vec4(textureSample.rgb, 1.0f);

	// out_color = vec4(vo_texcoord, 0.0f, 1.0f);
}
