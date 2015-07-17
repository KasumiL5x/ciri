#version 330

in vec3 vo_color;

layout (std140) uniform TestConstants {
	float alpha;
};

out vec4 out_color;

void main() {
	out_color = vec4(vo_color * alpha, 1.0f);
}
