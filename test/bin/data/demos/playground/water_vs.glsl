#version 440

layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_normal;
layout(location=2) in vec4 in_tangent;
layout(location=3) in vec2 in_texcoord;

layout(std140) uniform WaterConstants {
	mat4 world;
	mat4 xform;
	float ElapsedTime;
	vec3 CamPos;
};

out vec3 vo_nrm;
out vec2 vo_texcoord;
out mat4 vo_world;
out mat3 vo_invworld;
out vec3 vo_campos;
out mat4 vo_xform;

float WaveHeight = 3.5;
float WaveSpeed = 1.0;
float WaveLength = 10.5;
float RandomHeight = 0.5;
float RandomSpeed = 0.5;

float rand( vec3 co ) {
	return fract(sin(dot(co.xyz ,vec3(12.9898,78.233,45.5432))) * 43758.5453);
}

float rand2( vec3 co ) {
	return fract(sin(dot(co.xyz ,vec3(19.9128,75.2,34.5122))) * 12765.5213);
}

void main() {
	vec3 v0 = (world * vec4(in_position, 1.0)).xyz;
	float phase0 = (WaveHeight) * sin((ElapsedTime * WaveSpeed) + (v0.x * WaveLength) + (v0.z * WaveLength) + rand2(v0.xzz));
	float phase1 = (RandomHeight) * sin(cos(rand(v0.xzz) * RandomHeight * cos(ElapsedTime * RandomSpeed * sin(rand(v0.xxz)))));
	v0.y += phase0 + phase1;
	vec3 newPosition = mat3(transpose(world)) * v0;

	gl_Position = vec4(newPosition, 1.0);
	vo_nrm = in_normal;
	vo_texcoord = in_texcoord;
	vo_world = world;
	vo_invworld = mat3(transpose(world));
	vo_campos = CamPos;
	vo_xform = xform;
}
