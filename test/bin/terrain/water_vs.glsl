#version 330

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec4 in_tangent;
layout (location = 3) in vec2 in_texcoord;

layout (std140) uniform WaterConstants {
	mat4 world;
	mat4 worldview;
	mat4 xform;
	vec3 campos;
};

out vec3 vo_position;
out vec3 vo_normal;
out vec2 vo_texcoord;
out vec3 vo_campos;
out mat3 vo_tbn;

void main() {
	gl_Position = xform * vec4(in_position, 1.0f);
	vo_position = (world * vec4(in_position, 1.0f)).xyz;
	vo_normal = (world * vec4(in_normal, 0.0f)).xyz;
	vo_texcoord = in_texcoord;
	vo_campos = campos;

	vec3 n = normalize((world * vec4(in_normal, 0.0f)).xyz);
	vec3 t = normalize((world * vec4(in_tangent.xyz, 0.0f)).xyz);
	vec3 b = normalize((world * vec4((cross(in_normal, in_tangent.xyz) * in_tangent.w), 0.0f)).xyz);
	vo_tbn = mat3(t, b, n);
}
