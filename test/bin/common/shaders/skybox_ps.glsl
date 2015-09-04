#version 330

uniform samplerCube SkyboxTexture;

// in vec3 vo_texcoord;
smooth in vec3 vo_texcoord;

out vec4 out_color;

void main() {
	vec3 texSample = texture(SkyboxTexture, vo_texcoord).rgb;
	out_color = vec4(texSample, 1.0f);
}
