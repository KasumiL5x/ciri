#version 420

layout (std140) uniform SpriteConstants {
	mat4 projection;
};

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_texcoord;
layout (location = 2) in vec4 in_color;

out vec2 vo_texcoord;
out vec4 vo_color;

void main() {
	gl_Position = projection * vec4(in_position.xy, 0.0f, 1.0f);
	gl_Position.z = in_position.z;
	vo_texcoord = in_texcoord;
	vo_color = in_color;
}
