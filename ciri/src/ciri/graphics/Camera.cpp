#include <ciri/graphics/Camera.hpp>
#include <cc/MatrixFunc.hpp>

using namespace ciri::graphics;

Camera::Camera()
	: _view(1.0f), _viewDirty(true), _useViewOverride(false), _position(0.0f), _target(0.0f), _up(0.0f, 1.0f, 0.0f),
		_proj(1.0f), _projDirty(true), _useProjOverride(false), _fov(50.0f), _aspect(1.0f), _nearPlane(0.1f), _farPlane(1000.0f) {
}

Camera::~Camera() {
}

const cc::Mat4f& Camera::getView() {
	if( !_useViewOverride && _viewDirty ) {
		rebuildView();
		_viewDirty = false;
	}
	return _view;
}

const cc::Vec3f& Camera::getPosition() const {
	return _position;
}

const cc::Vec3f& Camera::getTarget() const {
	return _target;
}

const cc::Vec3f& Camera::getUp() const {
	return _up;
}

void Camera::setPosition( const cc::Vec3f& val ) {
	_position = val;
	_viewDirty = true;
}

void Camera::setTarget( const cc::Vec3f& val ) {
	_target = val;
	_viewDirty = true;
}

void Camera::setUp( const cc::Vec3f& val ) {
	_up = val;
	_viewDirty = true;
}

const cc::Mat4f& Camera::getProj() {
	if( !_useProjOverride && _projDirty ) {
		rebuildProj();
		_projDirty = false;
	}
	return _proj;
}

float Camera::getFov() const {
	return _fov;
}

float Camera::getAspect() const {
	return _aspect;
}

float Camera::getNearPlane() const {
	return _nearPlane;
}

float Camera::getFarPlane() const {
	return _farPlane;
}

void Camera::setFov( float val ) {
	_fov = val;
	_projDirty = true;
}

void Camera::setAspect( float val ) {
	_aspect = val;
	_projDirty = true;
}

void Camera::setPlanes( float near, float far ) {
	_nearPlane = near;
	_farPlane = far;
	_projDirty = true;
}

void Camera::setView( const cc::Mat4f& val ) {
	_view = val;
	_useViewOverride = true;
}

void Camera::setProj( const cc::Mat4f& val ) {
	_proj = val;
	_useProjOverride = true;
}

cc::Vec3f Camera::getForward() const {
	cc::Vec3f forward;
	forward.x = _target.x - _position.x;
	forward.y = _target.y - _position.y;
	forward.z = _target.z - _position.z;
	forward.normalize();
	return forward;
}

cc::Mat4f Camera::getRotation() const {
	const cc::Vec3f forward = getForward();
	const cc::Vec3f right = _up.cross(forward).normalized();
	return cc::Mat4f(right.x, _up.x, forward.x, 0.0f,
										right.y, _up.y, forward.y, 0.0f,
										right.z, _up.z, forward.z, 0.0f,
										0.0f,     0.0f, 0.0f,      1.0f);
}

cc::Mat4f Camera::getTranslation() const {
	return cc::Mat4f(1.0f, 0.0f, 0.0f, 0.0f,
										0.0f, 1.0f, 0.0f, 0.0f,
										0.0f, 0.0f, 1.0f, 0.0f,
										_position.x, _position.y, _position.z, 1.0f);
}

cc::Mat4f Camera::getWorld() const {
	return getTranslation() * getRotation();
}

void Camera::rebuildView() {
	_view = cc::math::lookAtRH(_position, _target, _up);
}

void Camera::rebuildProj() {
	_proj = cc::math::perspectiveRH(_fov, _aspect, _nearPlane, _farPlane);
}