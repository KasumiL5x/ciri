#include <ciri/gfx/FPSCamera.hpp>
#include <cc/MatrixFunc.hpp>

namespace ciri {
	FPSCamera::FPSCamera()
		: Camera(), _yaw(0.0f), _pitch(0.0f), _moveSpeed(1.0f), _rotateSensitivity(1.0f) {
	}

	FPSCamera::~FPSCamera() {
	}

	void FPSCamera::move( const cc::Vec3f& direction ) {
		const cc::Mat4f rotation = cc::math::rotate(_pitch, cc::Vec3f(1.0f, 0.0f, 0.0f)) *
															 cc::math::rotate(_yaw, cc::Vec3f(0.0f, 1.0f, 0.0f));
		const cc::Vec3f transDir = (cc::Vec4f(direction, 1.0f) * rotation).truncated(); // 0.0f ?
		_position += transDir * _moveSpeed;
		_viewDirty = true;
	}

	void FPSCamera::rotateYaw( float val ) {
		_yaw += val * _rotateSensitivity;
		_yaw = cc::math::wrapAngle(_yaw, 0.0f, 360.0f);
		_viewDirty = true;
	}

	void FPSCamera::rotatePitch( float val ) {
		_pitch += val * _rotateSensitivity;
		_pitch = cc::math::clamp(_pitch, -89.9f, 89.9f);
		_viewDirty = true;
	}

	void FPSCamera::setSensitivity( float move, float rotate ) {
		_moveSpeed = move;
		_rotateSensitivity = rotate;
	}

	void FPSCamera::setMoveSpeed( float val ) {
		_moveSpeed = val;
	}

	void FPSCamera::setYaw( float val ) {
		_yaw = val;
		_yaw = cc::math::wrapAngle(_yaw, 0.0f, 360.0f);
		_viewDirty = true;
	}

	void FPSCamera::setPitch( float val ) {
		_pitch = val;
		_pitch = cc::math::clamp(_pitch, -89.9f, 89.9f);
		_viewDirty = true;
	}

	float FPSCamera::getYaw() const {
		return _yaw;
	}

	float FPSCamera::getPitch() const {
		return _pitch;
	}


	void FPSCamera::rebuildView() {
		const cc::Mat4f rotation = cc::math::rotate(_pitch, cc::Vec3f(1.0f, 0.0f, 0.0f)) *
															 cc::math::rotate(_yaw, cc::Vec3f(0.0f, 1.0f, 0.0f));
		const cc::Vec3f transDir = (cc::Vec4f(0.0f, 0.0f, -1.0f, 0.0f) * rotation).truncated();
		const cc::Vec3f transUp = (cc::Vec4f(_up, 1.0f) * rotation).truncated();
		_view = cc::math::lookAt(_position, _position + transDir, transUp);
	}
} // ciri
