#version 420

layout (std140) uniform SpriteConstants {
	mat4 projection;
	vec2 screenSize;
};

layout (points) in;
layout (triangle_strip, max_vertices=6) out;

in vec2 vo_scale[];

out vec2 go_texcoord;

void generate_quad( float x, float y, float dx, float dy, float w, float h, float sinAngle, float cosAngle ) {
	vec2 TL = (projection * vec4(vec2(x+dx*cosAngle-(dy+h)*sinAngle, y+dx*sinAngle+(dy+h)*cosAngle), 0.0f, 1.0f)).xy;
	vec2 TR = (projection * vec4(vec2(x+(dx+w)*cosAngle-(dy+h)*sinAngle, y+(dx+w)*sinAngle+(dy+h)*cosAngle), 0.0f, 1.0f)).xy;
	vec2 BL = (projection * vec4(vec2(x+dx*cosAngle-dy*sinAngle, y+dx*sinAngle+dy*cosAngle), 0.0f, 1.0f)).xy;
	vec2 BR = (projection * vec4(vec2(x+(dx+w)*cosAngle-dy*sinAngle, y+(dx+w)*sinAngle+dy*cosAngle), 0.0f, 1.0f)).xy;

	vec2 TL_uv = vec2(0.0f, 1.0f);
	vec2 TR_uv = vec2(1.0f, 1.0f);
	vec2 BL_uv = vec2(0.0f, 0.0f);
	vec2 BR_uv = vec2(1.0f, 0.0f);

	// triangle 1
	gl_Position = vec4(TL, 0.0f, 1.0f); go_texcoord = TL_uv; EmitVertex();
	gl_Position = vec4(BR, 0.0f, 1.0f); go_texcoord = BR_uv; EmitVertex();
	gl_Position = vec4(BL, 0.0f, 1.0f); go_texcoord = BL_uv; EmitVertex();
	EndPrimitive();

	// triangle 2
	gl_Position = vec4(TL, 0.0f, 1.0f); go_texcoord = TL_uv; EmitVertex();
	gl_Position = vec4(TR, 0.0f, 1.0f); go_texcoord = TR_uv; EmitVertex();
	gl_Position = vec4(BR, 0.0f, 1.0f); go_texcoord = BR_uv; EmitVertex();
	EndPrimitive();
}

void main() {
	const float ROTATION = 0.0f;//0.785398163f;
	const float IMG_W = 64.0f;
	const float IMG_H = 62.0f;

	vec4 destinationRectangle = vec4(100.0f, 100.0f, IMG_W, IMG_H);
	vec2 origin = vec2(0.0f, 0.0f);

	vec2 worldOrigin;
	worldOrigin.x = origin.x * (destinationRectangle.z / IMG_W);
	worldOrigin.y = origin.y * (destinationRectangle.w / IMG_H);

	generate_quad(destinationRectangle.x,
								destinationRectangle.y,
								-worldOrigin.x,
								-worldOrigin.y,
								destinationRectangle.z,
								destinationRectangle.w,
								sin(ROTATION),
								cos(ROTATION));


}

// void main() {



// 	float sx = vo_scale[0].x;
// 	float sy = vo_scale[0].y;

// 	// a--d
// 	// |\ |
// 	// c--b

// 	vec4 center = projection * vec4(gl_in[0].gl_Position.xy * screenSize, 0.0f, 1.0f);

// 	vec4 a = vec4(center + vec4(-sx,  sy, 0.0f, 0.0f));
// 	vec4 b = vec4(center + vec4( sx, -sy, 0.0f, 0.0f));
// 	vec4 c = vec4(center + vec4(-sx, -sy, 0.0f, 0.0f));
// 	vec4 d = vec4(center + vec4( sx,  sy, 0.0f, 0.0f));

// 	gl_Position = a; go_texcoord = vec2(0.0f, 1.0f); EmitVertex();
// 	gl_Position = b; go_texcoord = vec2(1.0f, 0.0f); EmitVertex();
// 	gl_Position = c; go_texcoord = vec2(0.0f, 0.0f); EmitVertex();
// 	EndPrimitive();

// 	gl_Position = a; go_texcoord = vec2(0.0f, 1.0f); EmitVertex();
// 	gl_Position = d; go_texcoord = vec2(1.0f, 1.0f); EmitVertex();
// 	gl_Position = b; go_texcoord = vec2(1.0f, 0.0f); EmitVertex();
// 	EndPrimitive();
// }
