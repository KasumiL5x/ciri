#include "PlayerShip.hpp"

PlayerShip::PlayerShip()
	: Entity() {
}

PlayerShip::~PlayerShip() {
}

void PlayerShip::update( const cc::Vec2f& movement ) {
	const float SPEED = 8.0f;
	_velocity = movement * SPEED;
	_position += _velocity;
	if( _velocity.sqrMagnitude() > 0.0f ) {
		_orientation = atan2f(_velocity.y, _velocity.x);
	}
}