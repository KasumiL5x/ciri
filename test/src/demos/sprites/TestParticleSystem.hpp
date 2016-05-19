#ifndef __test_particle_system__
#define __test_particle_system__

#include <vector>
#include <cc/Vec2.hpp>
#include "IParticleSystem.hpp"
#include <ciri/Graphics.hpp>

class TestParticleSystem : public IParticleSystem {
public:
	TestParticleSystem();
	virtual ~TestParticleSystem();

	virtual void emitParticles( unsigned int count ) override;
	virtual void update( float deltaTime ) override;
	virtual void draw( SpriteBatch& spritebatch ) override;

	void setEmitterPosition( const cc::Vec2f& position );
	void setTexture( const std::shared_ptr<ciri::graphics::ITexture2D>& texture );
	void setEmitDirection( const cc::Vec2f& direction );

private:
	Particle& createParticle();
	cc::Vec2f generateSpawnPosition() const;
	float generateLifetime() const;
	cc::Vec2f generateVelocity() const;

private:
	std::vector<Particle> _particles;
	cc::Vec2f _emitterPosition;
	std::shared_ptr<ciri::graphics::ITexture2D> _texture;
	cc::Vec2f _emitDirection;
};

#endif /* __test_particle_system__ */