#ifndef __test_transform__
#define __test_transform__

#include <cc/Vec3.hpp>
#include <cc/Mat4.hpp>
#include <cc/Quaternion.hpp>

class Transform {
public:
	Transform();
	Transform( const Transform& rhs );
	Transform& operator=( const Transform& rhs );
	~Transform();

	const cc::Vec3f& getPosition() const;
	const cc::Vec3f& getScale() const;
	const cc::Quatf& getOrientation() const;
	const cc::Mat4f& getWorld();

	void setPosition( const cc::Vec3f& val );
	void setScale( const cc::Vec3f& val );
	void setOrientation( const cc::Quatf& val );
	void setWorld( cc::Mat4f& val );

	void identity();
	void rotate( const cc::Quatf& val );
	void translate( const cc::Vec3f& val );
	void translate( float x, float y, float z );
	void scale( const cc::Vec3f& val );
	void scale( float x, float y, float z );

private:
	cc::Vec3f _position;
	cc::Vec3f _scale;
	cc::Quatf _orientation;
	cc::Mat4f _world;
	bool _isWorldDirty;
};

#endif /* __test_transform__ */