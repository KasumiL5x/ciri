#ifndef __ciri_gfx_mayacamera__
#define __ciri_gfx_mayacamera__

#include "Camera.hpp"
#include <cc/Vec3.hpp>

namespace ciri {
	class MayaCamera : public Camera {
	public:
		MayaCamera();
		virtual ~MayaCamera();

		float getYaw() const;
		float getPitch() const;
		float getOffset() const;
		cc::Vec3f getLocalUp() const;
		cc::Vec3f getLocalRight() const;

		void update( float deltaTime );
		void setYaw( float val );
		void setPitch( float val );
		void setOffset( float val );
		void setMinOffset( float val );
		void setLerpStrength( float val );
		void setSensitivity( float rotate, float dolly, float pan );
		void rotateYaw( float val );
		void rotatePitch( float val );
		void dolly( float val );
		void resetPosition();
		void pan( float x, float y );

	private:
		float _yaw;
		float _pitch;
		float _offset;
		float _minOffset;
		float _lerpStrength;
		float _rotateSensitivity;
		float _dollySensitivity;
		float _panSensitivity;
	};
} // ciri

#endif /* __ciri_gfx_mayacamera__ */