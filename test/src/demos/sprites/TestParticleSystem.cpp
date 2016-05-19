#include "TestParticleSystem.hpp"
#include <cc/Random.hpp>

namespace gfx = ciri;

TestParticleSystem::TestParticleSystem()
	: IParticleSystem(), _emitterPosition(), _texture(nullptr), _emitDirection() {
}

TestParticleSystem::~TestParticleSystem() {
}

void TestParticleSystem::emitParticles( unsigned int count ) {
	for( unsigned int i = 0; i < count; ++i ) {
		Particle& p = createParticle();
		p.isAlive = true;
		p.position = generateSpawnPosition();
		p.texture = _texture;
		p.initialLifetime = generateLifetime();
		p.lifetime = p.initialLifetime;		
		p.velocity = generateVelocity();
		p.orientation = atan2f(p.velocity.normalized().y, p.velocity.normalized().x);
		p.initialTint = cc::Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
		p.tint = p.initialTint;
	}
}

void TestParticleSystem::update( float deltaTime ) {
	for( auto& p : _particles ) {
		if( !p.isAlive ) {
			continue;
		}

		p.lifetime -= deltaTime;
		if( p.lifetime < 0.0f ) {
			p.isAlive = false;
			continue;
		}

		p.position += p.velocity;

		const float percent = cc::math::percent<float>(0.0f, p.initialLifetime, p.lifetime);
		p.tint =  p.initialTint * percent;

		//todo: lifetime, border checks, etc.
	}
}

void TestParticleSystem::draw( SpriteBatch& spritebatch ) {
	if( nullptr == _texture ) {
		return;
	}

	for( auto& p : _particles ) {
		if( !p.isAlive || nullptr == p.texture ) {
			continue;
		}
		const cc::Vec2f origin = cc::Vec2f(static_cast<float>(p.texture->getWidth()) * 0.5f, static_cast<float>(p.texture->getHeight()) * 0.5f);
		const cc::Vec2f scale = cc::Vec2f(1.0f);
		spritebatch.draw(p.texture, p.position, p.orientation, origin, scale, 1.0f, p.tint);
		//spritebatch.draw(p.texture, p.position, rotation, origin, scale, 1.0f);
	}
}

void TestParticleSystem::setEmitterPosition( const cc::Vec2f& position ) {
	_emitterPosition = position;
}

void TestParticleSystem::setTexture( const std::shared_ptr<ciri::ITexture2D>& texture ) {
	_texture = texture;
	for( auto& p : _particles ) {
		p.texture = texture;
	}
}

void TestParticleSystem::setEmitDirection( const cc::Vec2f& direction ) {
	_emitDirection = direction;
}

Particle& TestParticleSystem::createParticle() {
	for( auto& p : _particles ) {
		if( p.isAlive ) {
			continue;
		}
		return p;
	}
	_particles.emplace_back(Particle());
	return _particles.back();
}

cc::Vec2f TestParticleSystem::generateSpawnPosition() const {
	// todo: add flux, etc.
	return _emitterPosition;
}

float TestParticleSystem::generateLifetime() const {
	//return cc::math::randRange<float>(5.0f, 5.0f);
	return cc::math::Random<float, int>::rangedReal(0.25f, 1.0f);
}

cc::Vec2f TestParticleSystem::generateVelocity() const {
	// http://www.playchilla.com/random-direction-in-2d
	cc::Vec2f direction = _emitDirection;
	const bool EMIT_DIRECTION_FLUX = true;

	if( EMIT_DIRECTION_FLUX ) {
		const float MIN_ANGLE = static_cast<float>(-cc::math::PI);
		const float MAX_ANGLE = static_cast<float>(cc::math::PI);
		const float angle = cc::math::Random<float, int>::rangedReal(MIN_ANGLE, MAX_ANGLE);
		direction.x += cosf(angle);
		direction.y += sinf(angle);
	}

	//const float U_MAX = 0.1f;
	//const float V_MAX = 0.1f;
	//if( EMIT_DIRECTION_FLUX ) {
	//	while( true ) {
	//		const float u = 2.0f * cc::math::randRange<float>(0.0f, U_MAX) - 1.0f;
	//		const float v = 2.0f * cc::math::randRange<float>(0.0f, V_MAX) - 1.0f;
	//		const float l = u*u + v*v;
	//		if( l > 1.0f || l < (0.0001f * 0.0001f) ) {
	//			continue;
	//		}
	//		direction += cc::Vec2f((u*u - v*v) / l, 2.0f * u * v / l);
	//		break;
	//	}
	//}
	direction.normalize();

	const float magnitude = cc::math::Random<float, int>::rangedReal(0.75f, 2.0f);
	return direction * magnitude;
}