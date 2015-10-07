#ifndef __particle_system__
#define __particle_system__

#include "Particle.hpp"
#include "SpriteBatch.hpp"
#include <cc/Vec2.hpp>

class IParticleSystem {
public:
	IParticleSystem();
	virtual ~IParticleSystem();

	virtual void emitParticles( unsigned int count )=0;
	virtual void update( float deltaTime )=0;
	virtual void draw( SpriteBatch& spritebatch )=0;
};

#endif /* __particle_system__ */