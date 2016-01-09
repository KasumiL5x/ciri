#ifndef __plane__
#define __plane__

#include <cc/Vec3.hpp>
#include <cc/Common.hpp>

struct Plane {
	Plane() {
		normal = cc::Vec3f::up();
		distance = 0.0f;
	}

	Plane( const cc::Vec3f& nrm, const float dist ) {
		normal = nrm;
		distance = dist;
	}

	cc::Vec3f normal;
	float distance;
};

static float sqrDistPointPlane( const Plane& plane, const cc::Vec3f& p ) {
return (plane.normal.dot(p) - plane.distance) / plane.normal.dot(plane.normal);
}

static bool isPointInFrontOfPlane( const Plane& plane, const cc::Vec3f& p ) {
	return sqrDistPointPlane(plane, p) > 0.0f;
}

static cc::Vec3f closestPointPlane( const Plane& plane, const cc::Vec3f& p ) {
    const float t = (plane.normal.dot(p) - plane.distance) / plane.normal.dot(plane.normal);
    return p - t * plane.normal;
}

static bool intersectSegmentPlane( const cc::Vec3f& a, const cc::Vec3f& b, const Plane& p, float& t, cc::Vec3f& q ) {
	const cc::Vec3f ab = b - a;
	const float d = p.normal.dot(ab);
	if( cc::math::equal<float>(d, 0.0f) ) {
		return false;
	}
	t = (p.distance - p.normal.dot(a)) / d;
	if( t >= 0.0f && t <= 1.0f ) {
		q = a + t * ab;
		return true;
	}
	return false;
}

//static bool intersectSegmentPlane( const cc::Vec3f& a, const cc::Vec3f& b, const Plane& p, float& t, cc::Vec3f& q ) {
//	const cc::Vec3f ab = b - a;
//	const float d = p.normal.dot(ab);
//	const float tmp1 = (p.distance - p.normal.dot(a));
//	const float tmp2 = p.normal.dot(ab);
//	t = (p.distance - p.normal.dot(a)) / p.normal.dot(ab);
//	if( t >= 0.0f && t <= 1.0f ) {
//		q = a + t * ab;
//		return true;
//	}
//	return false;
//}

#endif /* __plane__ */