#version 440

layout(std140) uniform DepthConstants {
	mat4 xform;
};

layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_normal;
layout(location=2) in vec4 in_tangent;
layout(location=3) in vec2 in_texcoord;

void main() {
	gl_Position = xform * vec4(in_position, 1.0);
}
