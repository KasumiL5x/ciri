#ifndef __clipmesh__
#define __clipmesh__

// NOTE THAT THIS IS HEAVILY BASED OFF GEOMETRIC TOOLS' CLIPMESH.PDF DOCUMENT AND CONVEXCLIPPER FROM WILDMAGIC5.

// Usage:
//    1. Provide an input Model that is already populated with vertices and indices.
//    2. Clip as many times as desired.
//    3. Convert to another Model.

// Notes:
//    - The input mesh MUST be a CLOSED convex polyhedron.  Anything else has undefined behavior.
//      As a result, all edges must have two and only two faces associated with them.

#include <vector>
#include <set>
#include <cc/Vec3.hpp>
#include "../../common/Model.hpp"
#include "ClipPlane.hpp"

class ClipMesh {
public:
	enum class Result {
		Dissected, /**< The mesh intersects the cutting plane.  New geometry will be created. */
		Visible,   /**< The mesh is entirely above the cutting plane.  The geometry will not change. */
		Invisibubble /**<  The mesh is entirely below the cutting plane.  The geometry will be discarded. */
	};

private:
	struct CVertex {
		cc::Vec3f point; /**< Position in 3d space. */
		float distance;  /**< Signed distance from cutting plane. */
		int occurs;      /**< Number of times vertex occurs (used in computing convex polygon resulting from clipping). */
		bool visible;    /**< True if vertex is on positive side of plane; false otherwise. */

		CVertex() {
			distance = 0.0f;
			visible = true;
		}
	};

	struct CEdge {
		int vertex[2]; /**< Indices of end point vertices of the edge. */
		int faces[2];  /**< Indices of faces shared by the edge. */
		bool visible;  /**< True if both end points are on the positive side of the plane; false otherwise, including when the edge is perfectly on the plane.  */

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
		std::set<int> edges; /**< Set of edges the face contains. */
		cc::Vec3f normal;    /**< Normal of the face.  Used for winding order detection. */
		bool visible;        /**< True if at least one of the face's edges is visible; false otherwise. */

		CFace() {
			visible = true;
		}
	};

	struct CEdgePlus {
		int E;  /**< Real edge this represents. */
		int V0; /**< Index of vertex 0 of the edge. */
		int V1; /**< Index of vertex 1 of the edge. */
		int F0; /**< Index of face 0 of the edge. */
		int F1; /**< Index of face 1 of the edge. */

		CEdgePlus() {
		}
		CEdgePlus( int e, const CEdge& edge );

		bool operator<( const CEdgePlus& rhs ) const;
		bool operator==( const CEdgePlus& rhs ) const;
		bool operator!=( const CEdgePlus& rhs ) const;
	};

public:
	/**
	 * Constructs by copying all required data from a source Model.
	 * @param sourceModel Original Model to parse.
	 */
	ClipMesh( Model& sourceModel );

	/**
	 * Clips the mesh with a plane.  Anything on the negative side of the plane will be discarded.
	 * @param clipPlane Plane to clip the mesh with.
	 * @returns Result indicating clipping status.
	 */
	Result clip( const ClipPlane& clipPlane );

	/**
	 * Converts the clipped mesh back to a regular Model.
	 * @param outModel Output Model to populate with converted data.
	 * @returns True upon success; false otherwise.
   */
	bool convert( Model* outModel );

	/**
	 * Prints debugging information about the current state of the mesh.
	 * @param verbose If true, prints out more detailed information.
	 */
	void printDebug( bool verbose );

private:
	/**
	 * Computes all vertices' signed distances to the clipping plane.
	 * @param clippingPlane Plane to compare the vertices to.
	 * @returns Result indicating clipping status.
	 */
	Result processVertices( const ClipPlane& clippingPlane );
	
	/**
	 * Modifies and splits edges based on their vertices' signed distances.
	 * Edges are split here, and also have their visiblity set and sometimes removed from faces.
	 */
	void processEdges();

	/**
	 * Fixes the faces with new edges and caps the newly created hole generated from the cutting process.
	 * @param clippingPlane Plane used to give the filled hole a normal.
	 */
	void processFaces( const ClipPlane& clippingPlane );
	
	/**
	 * Detects an open polyline's start and end points from a face.
	 * @param face   Face to check.
	 * @param vStart Output start point vertex index.
	 * @param vFinal Output end point vertex index.
	 * @returns True if polyline is open, false otherwise.
	 */
	bool getOpenPolyline( CFace& face, int& vStart, int& vFinal );

	/**
	 * TODO
	 */
	void postProcess( int fNew, CFace& faceNew );

	/**
	 * Return generated triangle indices.
	 * @param indices Output vector of indices.
	 */
	void getTriangles( std::vector<int>& indices );

	/**
	 * Order the vertices' indices into a new vector.
	 * @param face     Face to order vertices of.
	 * @param vOrdered Output vector of ordered vertex indices.
	 */
	void orderVertices( CFace& face, std::vector<int>& vOrdered );
	
	/**
	 * Swaps edges e0 and e1 in list.
	 * @param list List of indices.
	 * @param e0   Index of first edge to swap.
	 * @param e1   Index of second edge to swap.
	 */
	void swapEdges( std::vector<int>& list, int e0, int e1 );

private:
	std::vector<CVertex> _vertices;
	std::vector<CEdge> _edges;
	std::vector<CFace> _faces;
};

#endif /* __clipmesh__ */