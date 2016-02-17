#version 420

layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_normal;
layout(location=2) in vec4 in_tangent;
layout(location=3) in vec2 in_texcoord;

layout(std140) uniform ParallaxVertexConstants {
	mat4 world;
	mat4 xform;
	vec3 campos;
};


out vec3 vo_position;
out vec3 vo_normal;
out vec3 vo_viewdir;
out vec2 vo_texcoord;
out mat3 vo_tbn;

out vec3 vo_tangentLightPos;
out vec3 vo_tangentCamPos;
out vec3 vo_tangentFragPos;

// out vec3 vo_tangent;
// out vec3 vo_bitangent;

vec3 LightPosition = vec3(0.0, 15.0, 0.0);

void main() {
	gl_Position = xform * vec4(in_position, 1.0);

	vo_position = (world * vec4(in_position, 1.0)).xyz;
	vo_normal = (world * vec4(in_normal, 0.0)).xyz;
	vo_viewdir = (campos - vo_position);

	vec3 T = normalize(vec3(world * vec4(in_tangent.xyz, 0.0)));
	vec3 N = normalize(vec3(world * vec4(in_normal, 0.0)));
	vec3 bt = cross(N, T) * in_tangent.w;
	vec3 B = normalize(vec3(world * vec4(bt, 0.0)));
	vo_tbn = mat3(T, B, N);

	vo_tangentLightPos = vo_tbn * LightPosition;
	vo_tangentCamPos = vo_tbn * campos;
	vo_tangentFragPos = vo_tbn * vo_position;


	// vo_tangent = (world * vec4(in_tangent.xyz, 0.0f)).xyz;
	// vec3 bt = cross(in_normal, in_tangent.xyz) * in_tangent.w;
	// vo_bitangent = (world * vec4(bt, 0.0)).xyz;

	vo_texcoord = in_texcoord;
}

// old camera/eye space stuff
//
////// CONSTANTS
//
// layout(std140) uniform ParallaxVertexConstants {
// 	mat4 world;
// 	mat4 view;
// 	mat4 xform;
// };
//
////// OUTPUTS
//
// out vec3 vo_position_ws;
// out vec3 vo_eye_cs;
// out vec3 vo_lightdir_cs;
// out vec3 vo_normal_cs;
// out vec2 vo_texcoord;
//
////// SHADER
//
// // position of vertex in world space
// vo_position_ws = (world * vec4(in_position, 1.0)).xyz;
// // position of vertex in camera space
// vec3 position_cs = (view * world * vec4(in_position, 1.0)).xyz;
// // eye direction (from vertex to camera) in camera space (camera is origin in this space)
// vo_eye_cs = vec3(0.0, 0.0, 0.0) - position_cs;
// // light position in camera space
// vec3 lightpos_cs = (view * vec4(LightPosition, 1.0)).xyz;
// // light direction (from vertex to light) in camera space
// vo_lightdir_cs = lightpos_cs + vo_eye_cs;
// // normal of vertex in camera space
// vo_normal_cs = (view * world * vec4(in_normal, 0.0)).xyz;
