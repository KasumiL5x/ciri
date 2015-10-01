#include "Entity.hpp"

Entity::Entity()
	: _position(), _velocity(), _orientation(0.0f), _texture(nullptr), _origin(), _color(1.0f), _isAlive(false) {
}

Entity::~Entity() {
}

const cc::Vec2f& Entity::getPosition() const {
	return _position;
}

const cc::Vec2f& Entity::getVelocity() const {
	return _velocity;
}

const float& Entity::getOrientation() const {
	return _orientation;
}

const bool& Entity::isAlive() const {
	return _isAlive;
}

std::shared_ptr<ciri::ITexture2D> Entity::getTexture() const {
	return _texture;
}

const cc::Vec2f& Entity::getOrigin() const {
	return _origin;
}

const cc::Vec4f& Entity::getColor() const {
	return _color;
}

void Entity::setPosition( const cc::Vec2f& position ) {
	_position = position;
}

void Entity::setVelocity( const cc::Vec2f& velocity ) {
	_velocity = velocity;
}

void Entity::setOrientation( const float orientation ) {
	_orientation = orientation;
}

void Entity::setIsAlive( const bool alive ) {
	_isAlive = alive;
}

void Entity::setTexture( const std::shared_ptr<ciri::ITexture2D>& texture ) {
	if( nullptr == texture ) {
		return;
	}
	_texture = texture;
	_origin.x = static_cast<float>(texture->getWidth()) * 0.5f;
	_origin.y = static_cast<float>(texture->getHeight()) * 0.5f;
}

void Entity::setColor( const cc::Vec4f& color ) {
	_color = color;
}
