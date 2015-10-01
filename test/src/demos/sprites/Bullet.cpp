#include "Bullet.hpp"

Bullet::Bullet()
	: Entity() {
}

Bullet::~Bullet() {
}

void Bullet::update( const cc::Vec4f& bounds ) {
	if( _velocity.sqrMagnitude() > 0.0f ) {
		_orientation = atan2f(_velocity.y, _velocity.x);
	}

	_position += _velocity;

	if( (_position.x < bounds.x) ||
			(_position.x > bounds.z) ||
			(_position.y < bounds.y) ||
			(_position.y > bounds.w) ) {
		_isAlive = false;
	}
}