#ifndef __ciri_graphics_Camera__
#define __ciri_graphics_Camera__

#include <cc/Mat4.hpp>
#include <cc/Vec3.hpp>

namespace ciri { namespace graphics {

class Camera {
public:
	Camera();
	virtual ~Camera();

	const cc::Mat4f& getView();
	const cc::Vec3f& getPosition() const;
	const cc::Vec3f& getTarget() const;
	const cc::Vec3f& getUp() const;
	void setPosition( const cc::Vec3f& val );
	void setTarget( const cc::Vec3f& val );
	void setUp( const cc::Vec3f& val );

	const cc::Mat4f& getProj();
	float getFov() const;
	float getAspect() const;
	float getNearPlane() const;
	float getFarPlane() const;
	void setFov( float val );
	void setAspect( float val );
	void setPlanes( float near, float far );

	void setView( const cc::Mat4f& val );
	void setProj( const cc::Mat4f& val );

	cc::Vec3f getForward() const;
	cc::Mat4f getRotation() const;
	cc::Mat4f getTranslation() const;
	cc::Mat4f getWorld() const;

protected:
	virtual void rebuildView();
	virtual void rebuildProj();

protected:
	cc::Mat4f _view;
	bool _viewDirty;
	bool _useViewOverride;
	cc::Vec3f _position;
	cc::Vec3f _target;
	cc::Vec3f _up;
	//
	cc::Mat4f _proj;
	bool _projDirty;
	bool _useProjOverride;
	float _fov;
	float _aspect;
	float _nearPlane;
	float _farPlane;
};

}}

#endif