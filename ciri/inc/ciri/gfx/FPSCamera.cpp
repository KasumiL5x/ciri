#include <ciri/gfx/FPSCamera.hpp>
#include <cc/MatrixFunc.hpp>

namespace ciri {
	FPSCamera::FPSCamera()
		: Camera(), _yaw(0.0f), _pitch(0.0f), _moveSpeed(1.0f), _rotateSensitivity(1.0f) {
	}

	FPSCamera::~FPSCamera() {
	}

	void FPSCamera::move( Direction direction, float deltaTime ) {
		const float velocity = _moveSpeed * deltaTime;

		switch( direction ) {
			case Direction::Forward: {
				_position += _front * velocity;
				break;
			}
			case Direction::Backward: {
					_position -= _front * velocity;
					break;
			}
			case Direction::Left: {
				_position -= _right * velocity;
				break;
			}
			case Direction::Right: {
				_position += _right * velocity;
				break;
			}
		}
		_viewDirty = true;
	}

	void FPSCamera::rotateYaw( float val ) {
		_yaw += val * _rotateSensitivity;
		_yaw = cc::math::wrapAngle(_yaw, 0.0f, 360.0f);
		_viewDirty = true;
		updateVectors();
	}

	void FPSCamera::rotatePitch( float val ) {
		_pitch += val * _rotateSensitivity;
		_pitch = cc::math::clamp(_pitch, -89.9f, 89.9f);
		_viewDirty = true;
		updateVectors();
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
		updateVectors();
	}

	void FPSCamera::setPitch( float val ) {
		_pitch = val;
		_pitch = cc::math::clamp(_pitch, -89.9f, 89.9f);
		_viewDirty = true;
		updateVectors();
	}

	float FPSCamera::getYaw() const {
		return _yaw;
	}

	float FPSCamera::getPitch() const {
		return _pitch;
	}


	void FPSCamera::rebuildView() {
		_view = cc::math::lookAt(_position, _position + _front, _up);
	}

	void FPSCamera::updateVectors() {
		_front.x = cosf(cc::math::degreesToRadians(_yaw)) * cosf(cc::math::degreesToRadians(_pitch));
		_front.y = sinf(cc::math::degreesToRadians(_pitch));
		_front.z = sinf(cc::math::degreesToRadians(_yaw)) * cosf(cc::math::degreesToRadians(_pitch));
		_front.normalize();

		_right = _front.cross(cc::Vec3f::up()).normalized();

		_up = _right.cross(_front).normalized();
	}
} // ciri
