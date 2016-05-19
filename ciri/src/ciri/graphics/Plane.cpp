#include <ciri/graphics/Plane.hpp>

using namespace ciri::graphics;

Plane::Plane( const cc::Vec4f& val )
	: Plane(cc::Vec3f(val.x, val.y, val.z), val.w) {
}

Plane::Plane( const cc::Vec3f& normal, float d )
	: _normal(normal), _d(d) {
}

Plane::Plane( const cc::Vec3f& a, const cc::Vec3f& b, const cc::Vec3f& c ) {
	const cc::Vec3f ab = b - a;
	const cc::Vec3f ac = c - a;
	const cc::Vec3f cross = ab.cross(ac);
	_normal = cross.normalized();
	_d = -_normal.dot(a);
}

Plane::Plane( float a, float b, float c, float d )
	: Plane(cc::Vec3f(a, b, c), d) {
}

float Plane::dot( const cc::Vec4f& vec ) const {
	return ((((this->_normal.x * vec.x) + (this->_normal.y * vec.y)) + (this->_normal.z * vec.z)) + (this->_d * vec.w));
}

float Plane::dotCoord( const cc::Vec3f& vec ) const {
	return ((((this->_normal.x * vec.x) + (this->_normal.y * vec.y)) + (this->_normal.z * vec.z)) + this->_d);
}

float Plane::dotNormal( const cc::Vec3f& vec ) const {
	return (((this->_normal.x * vec.x) + (this->_normal.y * vec.y)) + (this->_normal.z * vec.z));
}

const cc::Vec3f& Plane::getNormal() const {
	return _normal;
}

float Plane::getD() const {
	return _d;
}