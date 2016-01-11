#ifndef __clipmesh__
#define __clipmesh__

// NOTE THAT THIS IS HEAVILY BASED OFF GEOMETRIC TOOLS' CLIPMESH.PDF DOCUMENT AND CONVEXCLIPPER.

// Usage:
//    1. Provide an input Model that is already populated with vertices and indices.
//    2. Clip as many times as desired.
//    3. Convert to another Model.

// Notes:
//    - The input mesh MUST be a CLOSED convex polyhedron.  Anything else has undefined behavior.
//      As a result, all edges must have two and only two faces associated with them.

#include <set>
#include <cc/Vec3.hpp>
#include "../../common/Model.hpp"
#include "Plane.hpp"

class ClipMesh {
public:
	enum class Result {
		Dissected, /**< The mesh intersects the cutting plane.  New geometry will be created. */
		Visible,   /**< The mesh is entirely above the cutting plane.  The geometry will not change. */
		Invisibubble /**<  The mesh is entirely below the cutting plane.  The geometry will be discarded. */
	};

private:
	struct CVertex {
		cc::Vec3f point;
		float distance;
		int occurs;
		bool visible;

		CVertex() {
			distance = 0.0f;
			visible = true;
		}
	};

	struct CEdge {
		int vertex[2];
		int faces[2];
		bool visible;

		CEdge() {
			visible = true;
		}
		CEdge& operator=( const CEdge& rhs ) {
			this->vertex[0] = rhs.vertex[0];
			this->vertex[1] = rhs.vertex[1];
			this->faces[0] = rhs.faces[0];
			this->faces[1] = rhs.faces[1];
			this->visible = rhs.visible;
			return *this;
		}
	};

	struct CFace {
		std::set<int> edges;
		cc::Vec3f normal;
		bool visible;

		CFace() {
			visible = true;
		}
	};

	struct CEdgePlus {
		int E;
		int V0;
		int V1;
		int F0;
		int F1;

		CEdgePlus() {
		}
		CEdgePlus( int e, const CEdge& edge );

		bool operator<( const CEdgePlus& rhs ) const;
		bool operator==( const CEdgePlus& rhs ) const;
		bool operator!=( const CEdgePlus& rhs ) const;
	};

public:
	ClipMesh( Model& sourceModel );

	Result clip( const Plane& clipPlane );
	bool convert( Model* outModel );
	void printDebug( bool verbose );

private:
	Result processVertices( const Plane& clippingPlane );
	void processEdges();
	void processFaces( const Plane& clippingPlane );
	//
	bool getOpenPolyline( CFace& face, int& vStart, int& vFinal );
	void postProcess( int fNew, CFace& faceNew );
	//
	void getTriangles( std::vector<int>& indices );
	void orderVertices( CFace& face, std::vector<int>& vOrdered );
	//
	void swapEdges( std::vector<int>& list, int e0, int e1 );

private:
	std::vector<CVertex> _vertices;
	std::vector<CEdge> _edges;
	std::vector<CFace> _faces;
};

#endif /* __clipmesh__ */