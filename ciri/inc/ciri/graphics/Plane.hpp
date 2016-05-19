#ifndef __ciri_graphics_Plane__
#define __ciri_graphics_Plane__

#include <cc/Vec3.hpp>
#include <cc/Vec4.hpp>

namespace ciri {

class Plane {
public:
	Plane( const cc::Vec4f& val );
	Plane( const cc::Vec3f& normal, float d );
	Plane( const cc::Vec3f& a, const cc::Vec3f& b, const cc::Vec3f& c );
	Plane( float a, float b, float c, float d );

	float dot( const cc::Vec4f& vec ) const;
	float dotCoord( const cc::Vec3f& vec ) const;
	float dotNormal( const cc::Vec3f& vec ) const;

	const cc::Vec3f& getNormal() const;
	float getD() const;

private:
	cc::Vec3f _normal;
	float _d;
};

}

#endif