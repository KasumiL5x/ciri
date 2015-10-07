#ifndef __particle__
#define __particle__

#include <cc/Vec2.hpp>
#include <cc/Vec4.hpp>
#include <ciri/Graphics.hpp>

struct Particle {
	Particle()
		: texture(nullptr), position(0.0f, 0.0f), velocity(0.0f, 0.0f), isAlive(false), lifetime(1.0f), initialLifetime(lifetime), tint(1.0f), initialTint(tint) {
	}
	virtual ~Particle() {
	}

	std::shared_ptr<ciri::ITexture2D> texture;
	cc::Vec2f position;
	cc::Vec2f velocity;
	bool isAlive;
	float lifetime;
	float initialLifetime;
	cc::Vec4f tint;
	cc::Vec4f initialTint;
};

#endif /* __particle__ */