#include "Plane.hpp"
#include <cc/MatrixFunc.hpp>

Plane::Plane()
	: _position(), _orientation(), _controller(nullptr), _world(), _worldDirty(false) {
}

Plane::~Plane() {
}

void Plane::setController( const std::shared_ptr<PlaneController>& newController ) {
	_controller = newController;
}

void Plane::throttle( float amount ) {
	const cc::Vec3f add = cc::Vec3f::forward() * _orientation;
	_position += add * amount;
	_worldDirty = true;
}

void Plane::roll( float amount ) {
	const auto rot = cc::Quatf::angleAxis(cc::Vec3f::forward(), amount);
	_orientation = rot * _orientation;
	_worldDirty = true;
}

void Plane::pitch( float amount ) {
	const auto rot = cc::Quatf::angleAxis(cc::Vec3f::right(), amount);
	_orientation = rot * _orientation;
	_worldDirty = true;
}

void Plane::setPosition( const cc::Vec3f& val ) {
	_position = val;
}

const cc::Mat4f& Plane::world() {
	if( _worldDirty ) {
		const cc::Mat4f translation = cc::math::translate<float>(_position);
		const cc::Mat4f rotation = cc::Quatf::createMatrixFromQuaternion(_orientation);
		const cc::Mat4f scale = cc::math::scale<float>(cc::Vec3f(1.0f, 1.0f, 1.0f));
		_world = translation * rotation * scale;
	}
	return _world;
}

const cc::Quatf& Plane::orientation() const {
	return _orientation;
}

const cc::Vec3f& Plane::position() const {
	return _position;
}