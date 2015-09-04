#version 330

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec4 in_tangent;
layout (location = 3) in vec2 in_texcoord;

layout (std140) uniform SkyboxConstants {
	mat4 view;
	mat4 invProj;
};

smooth out vec3 vo_texcoord;

void main() {
	mat3 inverseModelView = transpose(mat3(view));
	vec3 unprojected = (invProj * vec4(in_position, 1.0f)).xyz;
	vo_texcoord = inverseModelView * unprojected;
	vo_texcoord.y = -vo_texcoord.y;
	gl_Position = vec4(in_position, 1.0f);
}
