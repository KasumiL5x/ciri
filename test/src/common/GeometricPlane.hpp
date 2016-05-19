#ifndef __geometric_plane__
#define __geometric_plane__

#include "Vertex.hpp"
#include "Transform.hpp"
#include <ciri/Graphics.hpp>

class GeometricPlane {
public:
	GeometricPlane();
	GeometricPlane( const cc::Vec3f& p0, const cc::Vec3f& p1, const cc::Vec3f& p2, const cc::Vec3f& p3 );

	cc::Vec3f getNormal();
	cc::Vec3f getCenter();
	float getConstant();
	Transform& getXform();

	bool build( std::shared_ptr<ciri::graphics::IGraphicsDevice>& device );
	std::shared_ptr<ciri::graphics::IVertexBuffer> getVertexBuffer() const;
	std::shared_ptr<ciri::graphics::IIndexBuffer> getIndexBuffer() const;



private:
	Vertex _verts[4];
	std::shared_ptr<ciri::graphics::IVertexBuffer> _vb;
	std::shared_ptr<ciri::graphics::IIndexBuffer> _ib;
	Transform _xform;
};

#endif /* __geometric_plane__ */