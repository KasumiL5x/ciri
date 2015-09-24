#ifndef __spritevertex__
#define __spritevertex__

#include <cc/Vec3.hpp>
#include <cc/Vec2.hpp>

struct SpriteVertex {
	cc::Vec3f position;
	cc::Vec2f texcoord;

	SpriteVertex()
		: position(0.0f, 0.0f, 0.0f), texcoord(0.0f, 0.0f) {
	}

	SpriteVertex( const cc::Vec3f& pos, const cc::Vec2f& tex )
		: position(pos), texcoord(tex) {
	}
};

#endif /* __spritevertex__ */