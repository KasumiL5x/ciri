#ifndef __spritevertex__
#define __spritevertex__

#include <cc/Vec2.hpp>
#include <cc/Vec3.hpp>
#include <cc/Vec4.hpp>

struct SpriteVertex {
	cc::Vec3f position;
	cc::Vec2f texcoord;
	cc::Vec4f color;

	SpriteVertex()
		: position(0.0f, 0.0f, 0.0f), texcoord(0.0f, 0.0f), color(1.0f, 1.0f, 1.0f, 1.0f) {
	}

	SpriteVertex( const cc::Vec3f& pos, const cc::Vec2f& tex )
		: position(pos), texcoord(tex) {
	}

	SpriteVertex( const cc::Vec3f& pos, const cc::Vec2f& tex, const cc::Vec4f& col )
		: position(pos), texcoord(tex), color(col) {
	}
};

#endif /* __spritevertex__ */