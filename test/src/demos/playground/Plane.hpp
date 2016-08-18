#ifndef __playground_plane__
#define __playground_plane__

#include <memory>
#include <cc/Vec3.hpp>
#include <cc/Quaternion.hpp>
#include <cc/Mat4.hpp>

class PlaneController;

class Plane {
	friend class PlaneController;
public:
	Plane();
	~Plane();

	void setController( const std::shared_ptr<PlaneController>& newController );
	const cc::Mat4f& world();
	const cc::Quatf& orientation() const;
	const cc::Vec3f& position() const;

	void throttle( float amount );
	void roll( float amount );
	void pitch( float amount );

	void setPosition( const cc::Vec3f& val );

private:
	cc::Vec3f _position;
	cc::Quatf _orientation;
	std::shared_ptr<PlaneController> _controller;
	cc::Mat4f _world;
	bool _worldDirty;
};

#endif