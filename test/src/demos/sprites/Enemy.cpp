#include "Enemy.hpp"

Enemy::Enemy()
	: Entity() {
}

Enemy::~Enemy() {
}

void Enemy::update( const cc::Vec2f& screenSize ) {
	if( nullptr == _texture || nullptr == _target) {
		return;
	}

	followTarget(0.9f);

	_position += _velocity;

	// bounds limiting
	const cc::Vec2f size(static_cast<float>(_texture->getWidth()) * 0.5f, static_cast<float>(_texture->getHeight()) * 0.5f);
	_position.x = cc::math::clamp<float>(_position.x, size.x * 0.5f, screenSize.x - size.x * 0.5f);
	_position.y = cc::math::clamp<float>(_position.y, size.y * 0.5f, screenSize.y - size.y * 0.5f);

	_velocity *= 0.8f;
}

void Enemy::setTarget( const std::shared_ptr<Entity>& target ) {
	_target = target;
}

void Enemy::followTarget( const float acceleration ) {
	const cc::Vec2f diff = (_target->getPosition() - _position);
	const cc::Vec2f scaledDiff = diff * (acceleration / diff.magnitude());
	_velocity += scaledDiff;

	if( _velocity.sqrMagnitude() > 0.0f ) {
		_orientation = atan2f(_velocity.y, _velocity.x);
	}
}

Enemy Enemy::createSeeker( const cc::Vec2f& position ) {
	Enemy enemy;
	enemy.setPosition(position);
	return enemy;
}