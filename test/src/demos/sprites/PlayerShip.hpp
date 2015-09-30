#ifndef __player__
#define __player__

#include <cc/Vec2.hpp>
#include <ciri/Graphics.hpp>

class PlayerShip {
public:
	PlayerShip();
	~PlayerShip();

	void setTexture( std::shared_ptr<ciri::ITexture2D>& texture );
	void update( const cc::Vec2f& movement );

	const cc::Vec2f& getPosition() const;
	const cc::Vec2f& getVelocity() const;
	float getOrientation() const;
	std::shared_ptr<ciri::ITexture2D> getTexture() const;
	const cc::Vec2f& getOrigin() const;

	void setPosition( const cc::Vec2f& position );

private:
	cc::Vec2f _position;
	cc::Vec2f _velocity;
	float _orientation;

	std::shared_ptr<ciri::ITexture2D> _texture;
	cc::Vec2f _origin;
};

#endif /* __player__ */