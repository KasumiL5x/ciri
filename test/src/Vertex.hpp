#ifndef __test_vertex__
#define __test_vertex__

#include <cc/Vec2.hpp>
#include <cc/Vec3.hpp>

struct Vertex {
	cc::Vec3f position;
	cc::Vec3f normal;
	cc::Vec2f texcoord;

	Vertex()
		: position(), normal(), texcoord() {
	}

	Vertex( const cc::Vec3f& inPos, const cc::Vec3f& inNrm, const cc::Vec2f& inTex )
		: position(inPos), normal(inNrm), texcoord(inTex) {
	}
};

#endif /* __test_vertex__ */