#include <ciri/graphics/MayaCamera.hpp>
#include <cc/MatrixFunc.hpp>

using namespace ciri::graphics;

MayaCamera::MayaCamera()
	: Camera(), _yaw(0.0f), _pitch(0.0f), _offset(50.0f), _minOffset(1.0f), _lerpStrength(0.5f), _rotateSensitivity(1.0f),
		_dollySensitivity(1.0f), _panSensitivity(1.0f) {
}

MayaCamera::~MayaCamera() {
}

float MayaCamera::getYaw() const {
	return _yaw;
}

float MayaCamera::getPitch() const {
	return _pitch;
}

float MayaCamera::getOffset() const {
	return _offset;
}

cc::Vec3f MayaCamera::getLocalUp() const {
	const cc::Mat4f rotation = cc::math::rotate(_pitch, cc::Vec3f(1.0f, 0.0f, 0.0f)) *
															cc::math::rotate(_yaw, cc::Vec3f(0.0f, 1.0f, 0.0f));
	return (cc::Vec4f(0.0f, 1.0f, 0.0f, 0.0f) * rotation).truncated().normalized();
}

cc::Vec3f MayaCamera::getLocalRight() const {
	const cc::Mat4f rotation = cc::math::rotate(_pitch, cc::Vec3f(1.0f, 0.0f, 0.0f)) *
															cc::math::rotate(_yaw, cc::Vec3f(0.0f, 1.0f, 0.0f));
	return (cc::Vec4f(1.0f, 0.0f, 0.0f, 0.0f) * rotation).truncated().normalized();
}

void MayaCamera::update( float deltaTime ) {
	const cc::Mat4f rotation = cc::math::rotate(_pitch, cc::Vec3f(1.0f, 0.0f, 0.0f)) *
															cc::math::rotate(_yaw, cc::Vec3f(0.0f, 1.0f, 0.0f));
	cc::Vec3f forward = (cc::Vec4f(0.0f, 0.0f, 1.0f, 0.0f) * rotation).truncated();
	forward.normalize();
	forward *= _offset;
	const cc::Vec3f desiredPosition = _target + forward;
	_position = _position.lerp(desiredPosition, _lerpStrength * deltaTime);

	_viewDirty = true;
}
	
void MayaCamera::setYaw( float val ) {
	_yaw = val;
	_yaw = cc::math::wrapAngle(_yaw, 0.0f, 360.0f);
	_viewDirty = true;
}
	
void MayaCamera::setPitch( float val ) {
	_pitch = val;
	_pitch = cc::math::clamp(_pitch, -89.9f, 89.9f);
	_viewDirty = true;
}
	
void MayaCamera::setOffset( float val ) {
	_offset = fabsf(val);
	_offset = (_offset < _minOffset) ? _minOffset : _offset;
	_viewDirty = true;
}
	
void MayaCamera::setMinOffset( float val ) {
	_minOffset = val;
	// Set offset to itself to trigger the update.
	setOffset(_offset);
}
	
void MayaCamera::setLerpStrength( float val ) {
	_lerpStrength = fabsf(val);
}
	
void MayaCamera::setSensitivity( float rotate, float dolly, float pan ) {
	_rotateSensitivity = fabsf(rotate);
	_dollySensitivity = fabsf(dolly);
	_panSensitivity = fabsf(pan);
}
	
void MayaCamera::rotateYaw( float val ) {
	_yaw += val * _rotateSensitivity;
	_yaw = cc::math::wrapAngle(_yaw, 0.0f, 360.0f);
	_viewDirty = true;
}
	
void MayaCamera::rotatePitch( float val ) {
	_pitch += val * _rotateSensitivity;
	_pitch = cc::math::clamp(_pitch, -89.9f, 89.9f);
	_viewDirty = true;
}
	
void MayaCamera::dolly( float val ) {
	_offset += val * _dollySensitivity;
	_offset = (_offset < _minOffset) ? _minOffset : _offset;
	_viewDirty = true;
}
	
void MayaCamera::resetPosition() {
	const cc::Mat4f rotation = cc::math::rotate(_pitch, cc::Vec3f(1.0f, 0.0f, 0.0f)) *
															cc::math::rotate(_yaw, cc::Vec3f(0.0f, 1.0f, 0.0f));
	cc::Vec3f forward = (cc::Vec4f(0.0f, 0.0f, 1.0f, 0.0f) * rotation).truncated();
	forward.normalize();
	forward *= _offset;
	const cc::Vec3f desiredPosition = _target + forward;
	_position = desiredPosition;

	_viewDirty = true;
}

void MayaCamera::pan( float x, float y ) {
	const cc::Vec3f right = getLocalRight();
	_position += right * (x * _panSensitivity);
	_target += right * (x * _panSensitivity);

	const cc::Vec3f up = getLocalUp();
	_position += up * (y * _panSensitivity);
	_target += up * (y * _panSensitivity);
}