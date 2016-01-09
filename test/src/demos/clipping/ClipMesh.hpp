#ifndef __clipmesh__
#define __clipmesh__

#include <set>
#include <vector>
#include <cc/Vec3.hpp>
#include "../../common/Model.hpp"
#include "Plane.hpp"

// http://geometrictools.com/Documentation/ClipMesh.pdf
class ClipMesh {
private:
	struct CVertex {
		cc::Vec3f point;
		float distance; // signed distance from vertex to clip plane
		int occurs; // support for copmuting convex polygon resulting from clipping
		bool visible; // side of plane vertex is on (true=nonnegative; false=nonpositive)

		CVertex() {
			distance = 0.0;
			visible = true;
		}
	};

	struct CEdge {
		int vertex[2];
		std::vector<int> face;
		bool visible; // true if one end is on positive side and other on negative, false otherwise (including completely positive)

		bool operator==( const CEdge& rhs ) {
			if( vertex[0] != rhs.vertex[0] || vertex[1] != rhs.vertex[1] ) {
				return false;
			}

			if( face.size() != rhs.face.size() ) {
				return false;
			}

			for( int i = 0; i < face.size(); ++i ) {
				if( face[i] != rhs.face[i] ) {
					return false;
				}
			}

			if( visible != rhs.visible ) {
				return false;
			}

			return true;
		}

		bool operator!=( const CEdge& rhs ) {
			return !(*this == rhs);
		}

		CEdge() {
			visible = true;
		}
		CEdge( const CEdge& rhs ) {
			this->vertex[0] = rhs.vertex[0];
			this->vertex[1] = rhs.vertex[1];
			this->face = rhs.face;
			this->visible = rhs.visible;
		}
		CEdge& operator=( CEdge& rhs ) {
			this->vertex[0] = rhs.vertex[0];
			this->vertex[1] = rhs.vertex[1];
			this->face = rhs.face;
			this->visible = rhs.visible;
			return *this;
		}
	};

	struct CFace {
		std::set<int> edge;
		bool visible; // true if at least one edge is visible
		cc::Vec3f plane; // a.k.a surface normal

		CFace() {
			visible = true;
		}
	};

public:
	ClipMesh( Model& source );

	int clip( const Plane& clipPlane );
	void printDebug( bool verbose );

	Model convert();
	Model convert2();

private:
	int processVertices( const Plane& clipPlane );
	void processEdges();
	void processFaces( const Plane& clipPlane );
	bool getOpenPolyline( CFace f, int& start, int& final );
	std::vector<int> getOrderedFaces();
	std::vector<int> getOrderedVertices( CFace f );
	cc::Vec3f getNormal( const std::vector<int>& vertices ) const;
	void swap( int a, int b );
	void getTriangles( std::vector<int>& indices );
	void orderVertices( CFace& face, std::vector<int>& vOrdered );

private:
	std::vector<CVertex> V;
	std::vector<CEdge> E;
	std::vector<CFace> F;
};

#endif /* __clipmesh__ */