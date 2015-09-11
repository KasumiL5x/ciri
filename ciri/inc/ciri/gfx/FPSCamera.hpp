#ifndef __ciri_gfx_fps_camera__
#define __ciri_gfx_fps_camera__

#include "Camera.hpp"
#include <cc/Vec3.hpp>

namespace ciri {
	class FPSCamera : public Camera {
	public:
		enum class Direction {
			Forward,
			Backward,
			Left,
			Right
		};

	public:
		FPSCamera();
		virtual ~FPSCamera();

		void move( Direction direction, float deltaTime );
		void rotateYaw( float val );
		void rotatePitch( float val );
		void setSensitivity( float move, float rotate );
		void setMoveSpeed( float val );
		void setYaw( float val );
		void setPitch( float val );

		float getYaw() const;
		float getPitch() const;

		virtual void rebuildView() override;

		void updateVectors();

		const cc::Vec3f& getFpsFront() {
			updateVectors();
			return _front;
		}

	private:
		float _yaw;
		float _pitch;
		float _moveSpeed;
		float _rotateSensitivity;
		cc::Vec3f _front;
		cc::Vec3f _right;
	};
} // ciri

#endif /* __ciri_gfx_fps_camera__ */