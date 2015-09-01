#ifndef __ciri_fpscamera__
#define __ciri_fpscamera__

#include "Camera.hpp"
#include <cc/Vec3.hpp>

namespace ciri {
	class FPSCamera : public Camera {
	public:
		FPSCamera();
		virtual ~FPSCamera();

		void move( const cc::Vec3f& direction );
		void rotateYaw( float val );
		void rotatePitch( float val );
		void setSensitivity( float move, float rotate );
		void setMoveSpeed( float val );
		void setYaw( float val );
		void setPitch( float val );

		float getYaw() const;
		float getPitch() const;

		virtual void rebuildView() override;

	private:
		float _yaw;
		float _pitch;
		float _moveSpeed;
		float _rotateSensitivity;
	};
} // ciri

#endif /* __ciri_fpscamera__ */
