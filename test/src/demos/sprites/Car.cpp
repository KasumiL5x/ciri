#include "Car.hpp"

Car::Car()
	: _texture(nullptr), _position(0.0f, 0.0f), _origin(0.0f, 0.0f), _rotation(0.0f),
		_speed(200.0f), _steer(4.5f), _maxSpeed(300.0f), _lateralFrictionFactor(1.0f), _backwardsFrictionFactor(1.0f), _slidingTollerance(1.0f),
		_acceleration(0.0f, 0.0f), _currentSpeed(0.0f), _velocity(0.0f, 0.0f), _isSliding(false) {
}

Car::~Car() {
}

void Car::setTexture( const std::shared_ptr<ciri::ITexture2D> texture ) {
	_texture = texture;
}

std::shared_ptr<ciri::ITexture2D> Car::getTexture() const {
	return _texture;
}

void Car::setPosition( float x, float y ) {
	_position.x = x;
	_position.y = y;
}

const cc::Vec2f& Car::getPosition() const {
	return _position;
}

void Car::setOrigin( float x, float y ) {
	_origin.x = x;
	_origin.y = y;
}

const cc::Vec2f& Car::getOrigin() const {
	return _origin;
}

float Car::getRotation() const {
	return -_rotation;
}

void Car::update( float throttleInput, float steetInput, float deltaTime ) {
	_acceleration = getForward() * throttleInput * _speed;
	const float steerAngle = (steetInput * _steer) * (_currentSpeed / _maxSpeed);

	const cc::Vec2f lateralVelocity = getRight() * _velocity.dot(getRight());
	const cc::Vec2f lateralFriction = -lateralVelocity * _lateralFrictionFactor;
	const cc::Vec2f backwardsFriction = -_velocity * _backwardsFrictionFactor;
	_velocity += (backwardsFriction + lateralFriction) * deltaTime;

	_isSliding = lateralVelocity.magnitude() > _slidingTollerance;

	_currentSpeed = _velocity.magnitude();
	if( _currentSpeed < _maxSpeed ) {
		_velocity += _acceleration * deltaTime;
	}
	_position += _velocity * deltaTime;
	_rotation += cc::math::DEG_TO_RAD * -steerAngle;

	if( _isSliding ) {
		// todo: add slip item
	}
}

cc::Vec2f Car::getForward() const {
	return cc::Vec2f(sinf(_rotation), cosf(_rotation));
}

cc::Vec2f Car::getRight() const {
	return cc::Vec2f(getForward().x, -getForward().y);
}