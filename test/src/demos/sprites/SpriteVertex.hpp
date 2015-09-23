#ifndef __spritevertex__
#define __spritevertex__

#include <cc/Vec2.hpp>

struct SpriteVertex {
	cc::Vec2f position;
	cc::Vec2f texcoord;

	SpriteVertex()
		: position(0.0f, 0.0f), texcoord(0.0f, 0.0f) {
	}

	SpriteVertex( const cc::Vec2f& pos, const cc::Vec2f& tex )
		: position(pos), texcoord(tex) {
	}
};

#endif /* __spritevertex__ */