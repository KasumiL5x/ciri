#version 420

layout (std140) uniform SpriteConstants {
	mat4 projection;
};

layout ( points) in;
layout ( triangle_strip, max_vertices = 6) out;

in vec2 vo_scale[];

out vec2 go_texcoord;

void main() {
	float sx = vo_scale[0].x;
	float sy = vo_scale[0].y;

	// a--d
	// |\ |
	// c--b

	vec4 center = projection * vec4(gl_in[0].gl_Position.xy, 0.0f, 1.0f);

	vec4 a = vec4(center + vec4(-sx,  sy, 0.0f, 0.0f));
	vec4 b = vec4(center + vec4( sx, -sy, 0.0f, 0.0f));
	vec4 c = vec4(center + vec4(-sx, -sy, 0.0f, 0.0f));
	vec4 d = vec4(center + vec4( sx,  sy, 0.0f, 0.0f));

	gl_Position = a; go_texcoord = vec2(0.0f, 1.0f); EmitVertex();
	gl_Position = b; go_texcoord = vec2(1.0f, 0.0f); EmitVertex();
	gl_Position = c; go_texcoord = vec2(0.0f, 0.0f); EmitVertex();
	EndPrimitive();

	gl_Position = a; go_texcoord = vec2(0.0f, 1.0f); EmitVertex();
	gl_Position = d; go_texcoord = vec2(1.0f, 1.0f); EmitVertex();
	gl_Position = b; go_texcoord = vec2(1.0f, 0.0f); EmitVertex();
	EndPrimitive();
}
