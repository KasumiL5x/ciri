#ifndef __car__
#define __car__

#include <memory>
#include <ciri/Graphics.hpp>
#include <cc/Vec2.hpp>

class Car {
public:
	Car();
	~Car();

	void setTexture( const std::shared_ptr<ciri::ITexture2D> texture );
	std::shared_ptr<ciri::ITexture2D> getTexture() const;
	void setPosition( float x, float y );
	const cc::Vec2f& getPosition() const;
	void setOrigin( float x, float y );
	const cc::Vec2f& getOrigin() const;
	float getRotation() const;

	void update( float throttleInput, float steerInput, float deltaTime );

private:
	cc::Vec2f getForward() const;
	cc::Vec2f getRight() const;

private:
	std::shared_ptr<ciri::ITexture2D> _texture;
	cc::Vec2f _position;
	cc::Vec2f _origin;
	float _rotation;

	float _speed;
	float _steer;
	float _maxSpeed;
	float _lateralFrictionFactor;
	float _backwardsFrictionFactor;
	float _slidingTollerance;

	cc::Vec2f _acceleration;
	float _currentSpeed;
	cc::Vec2f _velocity;
	bool _isSliding;
};

#endif /* __car__ */