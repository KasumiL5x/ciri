#ifndef __spritebatchitem__
#define __spritebatchitem__

#include <memory>
#include <ciri/Graphics.hpp>
#include "SpriteVertex.hpp"

struct SpriteBatchItem {
	SpriteVertex topLeft;
	SpriteVertex topRight;
	SpriteVertex bottomLeft;
	SpriteVertex bottomRight;
	std::shared_ptr<ciri::ITexture2D> texture;
	float depth;

	void set( float x, float y, float dx, float dy, float w, float h, float sinAngle, float cosAngle, float depth ) {
		topLeft.position     = cc::Vec3f(x+dx*cosAngle-(dy+h)*sinAngle,     y+dx*sinAngle+(dy+h)*cosAngle,     depth);
		topRight.position    = cc::Vec3f(x+(dx+w)*cosAngle-(dy+h)*sinAngle, y+(dx+w)*sinAngle+(dy+h)*cosAngle, depth);
		bottomLeft.position  = cc::Vec3f(x+dx*cosAngle-dy*sinAngle,         y+dx*sinAngle+dy*cosAngle,         depth);
		bottomRight.position = cc::Vec3f(x+(dx+w)*cosAngle-dy*sinAngle,     y+(dx+w)*sinAngle+dy*cosAngle,     depth);

		topLeft.texcoord     = cc::Vec2f(0.0f, 1.0f);
		topRight.texcoord    = cc::Vec2f(1.0f, 1.0f);
		bottomLeft.texcoord  = cc::Vec2f(0.0f, 0.0f);
		bottomRight.texcoord = cc::Vec2f(1.0f, 0.0f);
	}
};

#endif /* __spritebatchitem__ */