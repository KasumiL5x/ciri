#ifndef __clipmesh__
#define __clipmesh__

#include <set>
#include <cc/Vec3.hpp>
#include "../../common/Model.hpp"
#include "Plane.hpp"

class ClipMesh {
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
		//////std::set<int> faces;
		bool visible;

		CEdge() {
			visible = true;
		}
		CEdge& operator=( const CEdge& rhs ) {
			this->vertex[0] = rhs.vertex[0];
			this->vertex[1] = rhs.vertex[1];
			this->faces[0] = rhs.faces[0];
			this->faces[1] = rhs.faces[1];
			//////this->faces = rhs.faces;
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

	int clip( const Plane& clipPlane );
	bool convert( Model* outModel );
	void printDebug( bool verbose );

private:
	int processVertices( const Plane& clippingPlane );
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