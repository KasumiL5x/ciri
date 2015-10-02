#ifndef __mathhelper__
#define __mathhelper__

#include <cc/Vec2.hpp>
#include <cc/Quaternion.hpp>

namespace MathHelper {
	static cc::Vec2f fromPolar( float angle, float magnitude ) {
		return magnitude * cc::Vec2f(cosf(angle), sinf(angle));
	}

	static cc::Vec2f transformVec2Quat( const cc::Vec2f& value, const cc::Quatf& rotation ) {
		const cc::Vec3f rot1 = cc::Vec3f(rotation.x + rotation.x, rotation.y + rotation.y, rotation.z + rotation.z);
		const cc::Vec3f rot2 = cc::Vec3f(rotation.x, rotation.x, rotation.w);
		const cc::Vec3f rot3 = cc::Vec3f(1, rotation.y, rotation.z);
		const cc::Vec3f rot4 = rot1*rot2;
		const cc::Vec3f rot5 = rot1*rot3;

		cc::Vec2f v;
		v.x = (float)((double)value.x * (1.0 - (double)rot5.y - (double)rot5.z) + (double)value.y * ((double)rot4.y - (double)rot4.z));
		v.y = (float)((double)value.x * ((double)rot4.y + (double)rot4.z) + (double)value.y * (1.0 - (double)rot4.x - (double)rot5.z));
		return v;
	}

	static cc::Quatf quatYawPitchRoll( float yaw, float pitch, float roll ) {
		cc::Quatf quaternion;
		const float num9 = roll * 0.5f;
		const float num6 = sinf(num9);
		const float num5 = cosf(num9);
		const float num8 = pitch * 0.5f;
		const float num4 = sinf(num8);
		const float num3 = cosf(num8);
		const float num7 = yaw * 0.5f;
		const float num2 = sinf(num7);
		const float num = cosf(num7);
		quaternion.x = ((num * num4) * num5) + ((num2 * num3) * num6);
		quaternion.y = ((num2 * num3) * num5) - ((num * num4) * num6);
		quaternion.z = ((num * num3) * num6) - ((num2 * num4) * num5);
		quaternion.w = ((num * num3) * num5) + ((num2 * num4) * num6);
		return quaternion;
	}
}

#endif /* __mathhelper__ */