#ifndef __bullet__
#define __bullet__

#include <cc/Vec2.hpp>
#include <cc/Vec4.hpp>
#include <ciri/Graphics.hpp>

class Bullet {
public:
	Bullet();

	void setTexture( std::shared_ptr<ciri::ITexture2D>& texture );
	void update( const cc::Vec4f& bounds );

	const cc::Vec2f& getPosition() const;
	const cc::Vec2f& getVelocity() const;
	float getOrientation() const;
	std::shared_ptr<ciri::ITexture2D> getTexture() const;
	const cc::Vec2f& getOrigin() const;
	bool isAlive() const;

	void setPosition( const cc::Vec2f& position );
	void setVelocity( const cc::Vec2f& velocity );
	void setIsAlive( bool alive );

private:
	cc::Vec2f _position;
	cc::Vec2f _velocity;
	float _orientation;
	bool _isAlive;

	std::shared_ptr<ciri::ITexture2D> _texture;
	cc::Vec2f _origin;
};

#endif /* __bullet__ */