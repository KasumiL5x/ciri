#version 420

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec4 in_tangent;
layout (location = 3) in vec2 in_texcoord;

layout (std140) uniform WaterConstants {
	mat4 world;
	mat4 xform;
	mat4 reflectedViewProj;
	vec3 campos;
	float time;
};

out vec3 vo_position;
out vec3 vo_normal;
out vec3 vo_campos;
out vec2 vo_bumpTexcoords;
out vec3 vo_cubeTexcoords;
out float vo_waveHeight;
out vec4 vo_reflectionTexcoords;
out vec4 vo_refractionTexcoords;

// -*- water variables -*-
float waveLength = 0.5f;
float waveHeight = 0.3f;
float windForce = 0.008f;
vec3 windDirection = normalize(vec3(0, 0, 1));

void main() {
	gl_Position = xform * vec4(in_position, 1.0f);

	// world position
	vo_position = (world * vec4(in_position, 1.0f)).xyz;

	// world normal
	vo_normal = (world * vec4(in_normal, 0.0f)).xyz;

	// camera position
	vo_campos = campos;

	// bumpmap perturbed texcoords
	vec3 perpDir = cross(windDirection, vec3(0, 1, 0));
	float ydot = dot(in_texcoord, windDirection.xz);
	float xdot = dot(in_texcoord, perpDir.xz);
	vec2 moveVector = vec2(xdot, ydot);
	moveVector += time * windForce;
	vo_bumpTexcoords = moveVector / waveLength;

	// cubemap texcoords
	vec3 V = normalize(vo_position - campos);
	vec3 RV = normalize(reflect(V, vo_normal));
	vo_cubeTexcoords = vec3(RV.x, -RV.y, RV.z);

	// calculate reflection texcoords
	mat4 reflectionMatrix = reflectedViewProj * world;
	vo_reflectionTexcoords = reflectionMatrix * vec4(in_position, 1.0f);

	// calculate refraction texcoords
	vo_refractionTexcoords = xform * vec4(in_position, 1.0f);

	// water variables
	vo_waveHeight = waveHeight;
}
