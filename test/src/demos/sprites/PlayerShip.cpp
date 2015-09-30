#include "PlayerShip.hpp"

PlayerShip::PlayerShip()
	: _position(), _velocity(), _orientation(0.0f), _texture(nullptr) {
}

PlayerShip::~PlayerShip() {
}


void PlayerShip::setTexture( std::shared_ptr<ciri::ITexture2D>& texture ) {
	if( nullptr == texture ) {
		return;
	}
	_texture = texture;
	_origin = cc::Vec2f(static_cast<float>(texture->getWidth()) * 0.5f, static_cast<float>(texture->getHeight()) * 0.5f);
}

void PlayerShip::update( const cc::Vec2f& movement ) {
	const float SPEED = 8.0f;
	_velocity = movement * SPEED;
	_position += _velocity;
	if( _velocity.sqrMagnitude() > 0.0f ) {
		_orientation = atan2f(_velocity.y, _velocity.x);
	}
}

const cc::Vec2f& PlayerShip::getPosition() const {
	return _position;
}

const cc::Vec2f& PlayerShip::getVelocity() const {
	return _velocity;
}

float PlayerShip::getOrientation() const {
	return _orientation;
}

std::shared_ptr<ciri::ITexture2D> PlayerShip::getTexture() const {
	return _texture;
}

const cc::Vec2f& PlayerShip::getOrigin() const {
	return _origin;
}

void PlayerShip::setPosition( const cc::Vec2f& position ) {
	_position = position;
}