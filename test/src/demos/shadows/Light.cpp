#include "Light.hpp"
#include <cc/MatrixFunc.hpp>
#include "BoundingBox.hpp"

Light::Light( Type type )
	: _diffuseColor(1.0f), _diffuseIntensity(1.0f), _specularColor(1.0f), _specularIntensity(1.0f),
		_type(type), _position(0.0f), _direction(0.0f, -0.99f, 0.0f), _range(10.0f), _coneInnerAngle(10.0f), _coneOuterAngle(12.0f), _castShadows(false),
		_viewDirty(true), _projDirty(true) {
}

Light::~Light() {
}


const cc::Vec3f& Light::diffuseColor() const {
	return _diffuseColor;
}

float Light::diffuseIntensity() const {
	return _diffuseIntensity;
}

const cc::Vec3f& Light::specularColor() const {
	return _specularColor;
}

float Light::specularIntensity() const {
	return _specularIntensity;
}

const cc::Vec3f& Light::position() const {
	return _position;
}

Light::Type Light::type() const {
	return _type;
}

const cc::Vec3f& Light::direction() const {
	return _direction;
}

float Light::range() const {
	return _range;
}

float Light::coneInnerAngle() const {
	return _coneInnerAngle;
}

float Light::coneOuterAngle() const {
	return _coneOuterAngle;
}

float Light::cosConeInnerAngle( bool radians ) const {
	return cosf(radians ? (_coneInnerAngle * cc::math::DEG_TO_RAD) : _coneInnerAngle);
}

float Light::cosConeOuterAngle( bool radians ) const {
	return cosf(radians ? (_coneOuterAngle * cc::math::DEG_TO_RAD) : _coneOuterAngle);
}

bool Light::castShadows() const {
	return _castShadows;
}

const cc::Mat4f& Light::view() {
	if( _viewDirty ) {
		_viewDirty = false;
		if( _type != Type::Directional ) {
			_view = cc::math::lookAtRH(_position, _position + _direction, cc::Vec3f::up());
		}
	}
	return _view;
}

const cc::Mat4f& Light::proj() {
	if( _projDirty ) {
		_projDirty = false;
		if( _type != Type::Directional) {
			_proj= cc::math::perspectiveRH(_coneOuterAngle * 2.0f, 1.0f, 0.1f, _range);
		}
	}
	return _proj;
}


void Light::setDiffuseColor( const cc::Vec3f& val ) {
	_diffuseColor.x = fabsf(val.x);
	_diffuseColor.y = fabsf(val.y);
	_diffuseColor.z = fabsf(val.z);
}

void Light::setDiffuseIntensity( float val ) {
	_diffuseIntensity = fabsf(val);
}

void Light::setSpecularColor( const cc::Vec3f& val ) {
	_specularColor.x = fabsf(val.x);
	_specularColor.y = fabsf(val.y);
	_specularColor.z = fabsf(val.z);
}

void Light::setSpecularIntensity( float val ) {
	_specularIntensity = val;
}

void Light::setPosition( const cc::Vec3f& val ) {
	_position = val;
	_viewDirty = true;
}

void Light::setDirection( const cc::Vec3f& val ) {
	_direction = val + cc::Vec3f(1e-5f, 1e-5f, 1e-5f); // fudge to avoid exact zero
	_direction.normalize();
	_viewDirty = true;
}

void Light::setRange( float val ) {
	_range = val;
	_projDirty = true;
}

void Light::setConeInnerAngle( float val ) {
	_coneInnerAngle = val;
}

void Light::setConeOuterAngle( float val ) {
	_coneOuterAngle = val;
	_projDirty = true;
}

void Light::setCastShadows( bool val ) {
	_castShadows = val;
}

void Light::computeViewProjFromFrustum( const BoundingFrustum& frustum ) {
	std::vector<cc::Vec3f> corners;
	for( const auto& p : frustum.corners() ) {
		corners.push_back(p);
	}

	cc::Vec3f center(0.0f, 0.0f, 0.0f);
	for( const auto& c : corners ) {
		center += c;
	}
	center /= static_cast<float>(corners.size());

	const float nearClipOffset = 0.0f;
	const float distFromCentroid = _range + nearClipOffset;
	const cc::Mat4f viewMatrix = cc::math::lookAtRH(center - (_direction * distFromCentroid), center, cc::Vec3f::up());

	for( auto& c : corners ) {
		c = (cc::Vec4f(c, 1.0f) * viewMatrix).truncated();
	}

	cc::Vec3f mins(std::numeric_limits<float>::max());
	cc::Vec3f maxs(std::numeric_limits<float>::min());
	for( const auto& c : corners ) {
		mins.x = (c.x < mins.x) ? c.x : mins.x;
		mins.y = (c.y < mins.y) ? c.y : mins.y;
		mins.z = (c.z < mins.z) ? c.z : mins.z;
		maxs.x = (c.x > maxs.x) ? c.x : maxs.x;
		maxs.y = (c.y > maxs.y) ? c.y : maxs.y;
		maxs.z = (c.z > maxs.z) ? c.z : maxs.z;
	}

	_proj = cc::math::orthographic<float>(-100.0f, 100.0f, -100.0f, 100.0f, -20.0f, 100.0f);
	//_proj = cc::math::orthographic<float>(mins.x, maxs.x, mins.y, maxs.y, -maxs.z - nearClipOffset, -mins.z);
	_view = cc::math::lookAtRH(cc::Vec3f::zero(), _direction, cc::Vec3f::up());
	//_proj = cc::math::orthographic<float>(-100.0f, 100.0f, -100.0f, 100.0f, -10.0f, 100.0f);
	//_view = viewMatrix;

	//// matrix that rotates points in direction of light
	//cc::Mat4f lightRotation = cc::math::lookAtRH(cc::Vec3f::zero(), -_direction, cc::Vec3f::up());
	//// corners of frustum
	//std::vector<cc::Vec3f> frustumCorners;
	//for( const auto& p : frustum.corners() ) {
	//	frustumCorners.push_back(p);
	//}
	//// transform corner positions into the direction of the light
	//for( auto& corner : frustumCorners ) {
	//	corner = (cc::Vec4f(corner, 1.0f) * lightRotation).truncated();
	//}
	//// find smallest bounding box around the points
	//BoundingBox lightBox = BoundingBox::createFromPoints(frustumCorners);
	//const cc::Vec3f boxSize = lightBox.Max - lightBox.Min;
	//const cc::Vec3f halfBoxSize = boxSize * 0.5f;
	//// position of light should be center of back panel of the box
	//cc::Vec3f lightPosition = lightBox.Min + halfBoxSize;
	//lightPosition.z = lightBox.Min.z;
	//// position back in world coordinates
	//lightRotation.invert();
	//lightPosition = (cc::Vec4f(lightPosition, 1.0f) * lightRotation).truncated();

	//// create view matrix
	////_view = cc::math::lookAtRH(lightPosition, lightPosition - _direction, cc::Vec3f::up());
	////_view = cc::math::lookAtRH(cc::Vec3f::zero(), _direction, cc::Vec3f::up());
	//// create projection matrix
	//const float near = -lightBox.Max.z;
	//const float far = -lightBox.Min.z;
	//_proj = cc::math::orthographic<float>(lightBox.Min.x, lightBox.Max.x, lightBox.Min.y, lightBox.Max.y, near, far);
	////_proj = cc::math::orthographic<float>(-100.0f, 100.0f, -100.0f, 100.0f, -10.0f, 100.0f);
}

void Light::computeViewProjOrtho( cc::Mat4f cameraView, float fov, float aspect, float nearP, float farP ) {
	cameraView.invert();
	const cc::Mat4f lightView = cc::math::lookAtRH(cc::Vec3f::zero(), _direction, cc::Vec3f::up());

	const float tanHalfHFov = tanf(cc::math::DEG_TO_RAD * (fov * 0.5f));
	const float tanHalfVFov = tanf(cc::math::DEG_TO_RAD * ((fov * aspect) * 0.5f));

	const float xn = nearP * tanHalfHFov;
	const float xf = farP * tanHalfHFov;
	const float yn = nearP * tanHalfVFov;
	const float yf = farP * tanHalfVFov;
	cc::Vec4f frustumCorners[8] = {
		cc::Vec4f( xn,  yn, nearP, 1.0f),
		cc::Vec4f(-xn,  yn, nearP, 1.0f),
		cc::Vec4f( xn, -yn, nearP, 1.0f),
		cc::Vec4f(-xn, -yn, nearP, 1.0f),
		cc::Vec4f( xf,  yf, farP, 1.0f),
		cc::Vec4f(-xf,  yf, farP, 1.0f),
		cc::Vec4f( xf, -yf, farP, 1.0f),
		cc::Vec4f(-xf, -yf, farP, 1.0f)
	};

	cc::Vec4f frustomCornersLocal[8];
	cc::Vec3f minimum(std::numeric_limits<float>::max());
	cc::Vec3f maximum(std::numeric_limits<float>::min());
	for( int i = 0; i < 8; ++i ) {
		const cc::Vec4f vW = frustumCorners[i] * cameraView;
		frustomCornersLocal[i] = vW * lightView;

		minimum.x = cc::math::minimum(minimum.x, frustomCornersLocal[i].x);
		minimum.y = cc::math::minimum(minimum.y, frustomCornersLocal[i].y);
		minimum.z = cc::math::minimum(minimum.z, frustomCornersLocal[i].z);
		maximum.x = cc::math::maximum(maximum.x, frustomCornersLocal[i].x);
		maximum.y = cc::math::maximum(maximum.y, frustomCornersLocal[i].y);
		maximum.z = cc::math::maximum(maximum.z, frustomCornersLocal[i].z);
	}

	_proj = cc::math::orthographic<float>(minimum.x, maximum.x, minimum.y, maximum.y, minimum.z, maximum.z);
	_view = cc::math::lookAtRH(cc::Vec3f::zero(), _direction, cc::Vec3f::up());
}
