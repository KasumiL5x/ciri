#include "Bullet.hpp"

Bullet::Bullet()
	: _position(), _velocity(), _orientation(0.0f), _isAlive(false), _texture(nullptr), _origin() {
}

void Bullet::setTexture( std::shared_ptr<ciri::ITexture2D>& texture ) {
	if( nullptr == texture ) {
		return;
	}
	_texture = texture;
	_origin = cc::Vec2f(static_cast<float>(texture->getWidth()) * 0.5f, static_cast<float>(texture->getHeight()) * 0.5f);
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

const cc::Vec2f& Bullet::getPosition() const {
	return _position;
}

const cc::Vec2f& Bullet::getVelocity() const {
	return _velocity;
}

float Bullet::getOrientation() const {
	return _orientation;
}

std::shared_ptr<ciri::ITexture2D> Bullet::getTexture() const {
	return _texture;
}

const cc::Vec2f& Bullet::getOrigin() const {
	return _origin;
}

bool Bullet::isAlive() const {
	return _isAlive;
}

void Bullet::setPosition( const cc::Vec2f& position ) {
	_position = position;
}

void Bullet::setVelocity( const cc::Vec2f& velocity ) {
	_velocity = velocity;
}

void Bullet::setIsAlive( bool alive ) {
	_isAlive = alive;
}