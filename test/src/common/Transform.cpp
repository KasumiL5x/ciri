#include "Transform.hpp"
#include <cc/MatrixFunc.hpp>

Transform::Transform()
	: _position(), _scale(1.0f), _orientation(), _world(), _isWorldDirty(true) {
}

Transform::Transform( const Transform& rhs ) {
	this->_position = rhs._position;
	this->_scale = rhs._scale;
	this->_orientation = rhs._orientation;
	this->_world = rhs._world;
	this->_isWorldDirty = rhs._isWorldDirty;
}

Transform& Transform::operator=( const Transform& rhs ) {
	this->_position = rhs._position;
	this->_scale = rhs._scale;
	this->_orientation = rhs._orientation;
	this->_world = rhs._world;
	this->_isWorldDirty = rhs._isWorldDirty;
	return *this;
}

Transform::~Transform() {
}

const cc::Vec3f& Transform::getPosition() const {
	return _position;
}

const cc::Vec3f& Transform::getScale() const {
	return _scale;
}

const cc::Quatf& Transform::getOrientation() const {
	return _orientation;
}

const cc::Mat4f& Transform::getWorld() {
	if( _isWorldDirty ) {
		const cc::Mat4f translation = cc::math::translate(_position);
		const cc::Mat4f rotation = cc::Quatf::createMatrixFromQuaternion(_orientation);
		const cc::Mat4f scale = cc::math::scale(_scale);
		_world = translation * rotation * scale;
		_isWorldDirty = false;
	}
	return _world;
}

void Transform::setPosition( const cc::Vec3f& val ) {
	_position = val;
	_isWorldDirty = true;
}

void Transform::setScale( const cc::Vec3f& val ) {
	_scale = val;
	_isWorldDirty = true;
}

void Transform::setOrientation( const cc::Quatf& val ) {
	_orientation = val;
	_isWorldDirty = true;
}

void Transform::setWorld( cc::Mat4f& val ) {
	_world = val;
	cc::math::decompose(_world, &_position, &_orientation, &_scale);
	_isWorldDirty = false;
}

void Transform::identity() {
	_position = cc::Vec3f();
	_scale = cc::Vec3f(1.0f);
	_orientation = cc::Quatf();
	_world = cc::Mat4f(1.0f);
	_isWorldDirty = false;
}

void Transform::rotate( const cc::Quatf& val ) {
	_orientation = _orientation * val;
	_isWorldDirty = true;
}

void Transform::translate( const cc::Vec3f& val ) {
	_position += val;
	_isWorldDirty = true;
}

void Transform::translate( float x, float y, float z ) {
	_position.x += x;
	_position.y += y;
	_position.z += z;
	_isWorldDirty = true;
}

void Transform::scale( const cc::Vec3f& val ) {
	_scale = val;
	_isWorldDirty = true;
}

void Transform::scale( float x, float y, float z ) {
	_scale.x += x;
	_scale.y += y;
	_scale.z += z;
	_isWorldDirty = true;
}