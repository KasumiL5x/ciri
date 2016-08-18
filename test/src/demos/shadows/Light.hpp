#ifndef __ShadowsDemo_Light__
#define __ShadowsDemo_Light__

#include <cc/Vec3.hpp>
#include <cc/Mat4.hpp>
#include "BoundingFrustum.hpp"

class Light {
public:
	enum class Type {
		Directional,
		Point,
		Spot,
		Invalid
	};

public:
	Light( Type type );
	~Light();

	const cc::Vec3f& diffuseColor() const;
	float diffuseIntensity() const;
	const cc::Vec3f& specularColor() const;
	float specularIntensity() const;
	const cc::Vec3f& position() const;
	Type type() const;
	const cc::Vec3f& direction() const;
	float range() const;
	float coneInnerAngle() const;
	float coneOuterAngle() const;
	float cosConeInnerAngle( bool radians ) const;
	float cosConeOuterAngle( bool radians ) const;
	bool castShadows() const;
	const cc::Mat4f& view();
	const cc::Mat4f& proj();

	void setDiffuseColor( const cc::Vec3f& val );
	void setDiffuseIntensity( float val );
	void setSpecularColor( const cc::Vec3f& val );
	void setSpecularIntensity( float val );
	void setPosition( const cc::Vec3f& val );
	void setDirection( const cc::Vec3f& val );
	void setRange( float val );
	void setConeInnerAngle( float val );
	void setConeOuterAngle( float val );
	void setCastShadows( bool val );
	void computeViewProjFromFrustum( const BoundingFrustum& frustum );
	void computeViewProjOrtho( cc::Mat4f cameraView, float fov, float aspect, float nearP, float farP );

private:
	// colors
	cc::Vec3f _diffuseColor;
	float _diffuseIntensity;
	cc::Vec3f _specularColor;
	float _specularIntensity;

	// general properties
	Type _type;
	cc::Vec3f _position;
	cc::Vec3f _direction;
	float _range;
	float _coneInnerAngle;
	float _coneOuterAngle;
	bool _castShadows;

	// matrices
	cc::Mat4f _view;
	bool _viewDirty;
	cc::Mat4f _proj;
	bool _projDirty;
};

#endif