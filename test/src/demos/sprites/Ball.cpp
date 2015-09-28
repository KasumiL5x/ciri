#include "Ball.hpp"

Ball::Ball() 
	: _texture(nullptr), _rotation(0.0f), _origin(0.0f), _position(0.0f), _velocity(0.0f), _active(false) {
}

Ball::~Ball() {
}

std::shared_ptr<ciri::ITexture2D> Ball::getTexture() const {
	return _texture;
}

bool Ball::setTexture( const std::shared_ptr<ciri::ITexture2D>& texture ) {
	if( nullptr == texture || 0 == texture->getWidth() || 0 == texture->getHeight() ) {
		return false;
	}

	_texture = texture;
	_origin.x = static_cast<float>(texture->getWidth()) * 0.5f;
	_origin.y = static_cast<float>(texture->getHeight()) * 0.5f;
	return true;
}

float Ball::getRotation() const {
	return _rotation;
}

void Ball::setRotation( float rotation ) {
	_rotation = rotation;
}

const cc::Vec2f& Ball::getOrigin() const {
	return _origin;
}

const cc::Vec2f& Ball::getPosition() const {
	return _position;
}

void Ball::setPosition( const cc::Vec2f& position ) {
	_position = position;
}

const cc::Vec2f& Ball::getVelocity() const {
	return _velocity;
}

void Ball::setVelocity( const cc::Vec2f& velocity ) {
	_velocity = velocity;
}

bool Ball::isActive() const {
	return _active;
}

void Ball::setActive( bool active ) {
	_active = active;
}