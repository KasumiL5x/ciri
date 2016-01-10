#include "NewClipMesh.hpp"
#include <algorithm>
#include <cc/TriMath.hpp>

NewClipMesh::CEdgePlus::CEdgePlus( int e, const CEdge& edge ) {
	this->E = e;
	this->faces = edge.faces;
	if( edge.vertex[0] < edge.vertex[1] ) {
		this->V0 = edge.vertex[0];
		this->V1 = edge.vertex[1];
	} else {
		this->V0 = edge.vertex[1];
		this->V1 = edge.vertex[0];
	}
}

bool NewClipMesh::CEdgePlus::operator<( const CEdgePlus& rhs ) const {
	if( this->V1 < rhs.V1 ) {
		return true;
	}
	if( this->V1 == rhs.V1 ) {
		return this->V0 < rhs.V0;
	}
	return false;
}

bool NewClipMesh::CEdgePlus::operator==( const CEdgePlus& rhs ) const {
	return (this->V0 == rhs.V0) && (this->V1 == rhs.V1);
}

bool NewClipMesh::CEdgePlus::operator!=( const CEdgePlus& rhs ) const {
	return (this->V0 != rhs.V0) || (this->V1 != rhs.V1);
}

NewClipMesh::NewClipMesh( Model& sourceModel ) {
	// copy vertices over
	const std::vector<Vertex>& srcVerts = sourceModel.getVertices();
	_vertices.resize(srcVerts.size());
	for( unsigned int i = 0; i < srcVerts.size(); ++i ) {
		_vertices[i].point = srcVerts[i].position;
	}
	
	// copy edges over
	const std::vector<Model::Edge>& srcEdges = sourceModel.getEdges();
	_edges.resize(srcEdges.size());
	for( unsigned int currEdge = 0; currEdge < srcEdges.size(); ++currEdge ) {
		_edges[currEdge].vertex[0] = srcEdges[currEdge].idx[0];
		_edges[currEdge].vertex[1] = srcEdges[currEdge].idx[1];
		for( unsigned int currFace = 0; currFace < srcEdges[currEdge].faces.size(); ++currFace ) {
			_edges[currEdge].faces.insert(srcEdges[currEdge].faces[currFace]);
		}
	}

	// copy faces over
	const std::vector<Model::Triangle>& srcTris = sourceModel.getTriangles();
	_faces.resize(srcTris.size());
	for( unsigned int currTri = 0; currTri < srcTris.size(); ++currTri ) {
		const Model::Triangle& tri = srcTris[currTri];
		const cc::Vec3f& p0 = srcVerts[tri.idx[0]].position;
		const cc::Vec3f& p1 = srcVerts[tri.idx[1]].position;
		const cc::Vec3f& p2 = srcVerts[tri.idx[2]].position;
		_faces[currTri].normal = cc::math::computeTriangleNormal(p0, p1, p2).normalized();
		for( unsigned int currEdge = 0; currEdge < tri.edges.size(); ++currEdge ) {
			_faces[currTri].edges.insert(tri.edges[currEdge]);
		}
	}
}

int NewClipMesh::clip( const Plane& clipPlane ) {
	const int result = processVertices(clipPlane);
	if( 1 == result ) {
		return 1;
	} else if( -1 == result ) {
		return -1;
	}

	processEdges();
	processFaces(clipPlane);

	return 0;
}

Model NewClipMesh::convert() {
	// get visible vertices
	const int numVertices = _vertices.size();
	std::vector<Vertex> points;
	int* vMap = new int[numVertices];
	memset(vMap, 0xFF, numVertices * sizeof(int));
	for( int currVtx = 0; currVtx < numVertices; ++currVtx ) {
		const CVertex& vtx = _vertices[currVtx];
		if( !vtx.visible ) {
			continue;
		}
		vMap[currVtx] = points.size();
		points.push_back(Vertex(vtx.point, cc::Vec3f(), cc::Vec2f()));
	}

	std::vector<int> indices;
	getTriangles(indices);

	// reorder the indices
	for( int currIdx = 0; currIdx < indices.size(); ++currIdx ) {
		const int oldIdx = indices[currIdx];
		assert(0 <= oldIdx && oldIdx < numVertices); // index out of range
		const int newIdx = vMap[oldIdx];
		assert(0 <= newIdx && newIdx < points.size()); // index out of range
		indices[currIdx] = newIdx;
	}
	delete[] vMap;

	Model mdl;
	for( unsigned int i = 0; i < points.size(); ++i ) {
		mdl.addVertex(points[i]);
	}
	for( unsigned int i = 0; i < indices.size(); ++i ) {
		mdl.addIndex(indices[i]);
	}
	return mdl;
}

void NewClipMesh::printDebug( bool verbose ) {
	int visibleVertices = 0;
	for( int i=0; i<_vertices.size(); ++i){if(_vertices[i].visible){visibleVertices+=1;}}
	printf("CVertex(%d, %d visible)\n", _vertices.size(), visibleVertices);
	if( verbose ) {
		for( int i = 0; i < _vertices.size(); ++i ) {
			printf("\t[%d] (%f, %f, %f); visible: %d\n", i, _vertices[i].point.x, _vertices[i].point.y, _vertices[i].point.z, _vertices[i].visible);
		}
	}

	int visibleEdges = 0;
	for( int i=0; i<_edges.size(); ++i){if(_edges[i].visible){visibleEdges+=1;}}
	printf("CEdge(%d, %d visible)\n", _edges.size(), visibleEdges);
	if( verbose ) {
		for( int i = 0; i < _edges.size(); ++i ) {
			printf("\t[%d] (%d, %d) connected to %d faces; visible: %d\n", i, _edges[i].vertex[0], _edges[i].vertex[1], _edges[i].faces.size(), _edges[i].visible);
		}
	}

	int visibleFaces = 0;
	for( int i=0; i<_faces.size(); ++i){if(_faces[i].visible){visibleFaces+=1;}}
	printf("CFace(%d, %d visible)\n", _faces.size(), visibleFaces);
	if( verbose ) {
		for( int i = 0; i < _faces.size(); ++i ) {
			printf("\t[%d] has %d edges; visible: %d\n", i, _faces[i].edges.size(), _faces[i].visible);
		}
	}
}

int NewClipMesh::processVertices( const Plane& clippingPlane ) {
	const float EPSILON = 0.00001f;
	int numPositive = 0;
	int numNegative = 0;
	int numZero = 0;
	
	// compute signed distance from vertices to plane
	const int numVertices = _vertices.size();
	for( unsigned int currVert = 0; currVert < numVertices; ++currVert ) {
		CVertex& vtx = _vertices[currVert];
		if( !vtx.visible ) {
			continue;
		}

		vtx.distance = clippingPlane.normal.dot(vtx.point) - clippingPlane.distance;
		//vtx.distance = sqrDistPointPlane(clippingPlane, vtx.point);
		if( vtx.distance > EPSILON ) {
			++numPositive;
		} else if( vtx.distance < -EPSILON ) {
			++numNegative;
			vtx.visible = false;
		} else {
			// point is on plane within tolerance
			++numZero;
			vtx.distance = 0.0f;
		}
	}

	// mesh is in negative halfspace, fully clipped
	if( 0 == numPositive ) {
		return -1;
	}

	// mesh is in positive halfspace, fully visible
	if( 0 == numNegative ) {
		return 1;
	}

	// clipped
	return 0;
}

void NewClipMesh::processEdges() {
	const int numEdges = _edges.size();
	for( unsigned int currEdge = 0; currEdge < numEdges; ++currEdge ) {
		CEdge& edge = _edges[currEdge];
		if( !edge.visible ) {
			continue;
		}

		const float d0 = _vertices[edge.vertex[0]].distance;
		const float d1 = _vertices[edge.vertex[1]].distance;
		if( d0 <= 0.0f && d1 <= 0.0f ) {
			// the edge is culled.  if the edge is exactly on the clip plane,
			// it is possilbe that a visible triangle shares it.  the edge will
			// be re-added during the face processsing
			std::set<int>::const_iterator iter = edge.faces.begin();
			const std::set<int>::const_iterator end = edge.faces.end();
			while( iter != end ) {
				const int idx = *iter++;
				CFace& face = _faces[idx];
				face.edges.erase(currEdge);
				if( face.edges.empty() ) {
					face.visible = false;
				}
			}

			edge.visible = false;
			continue;
		} //endif

		// face is on nonnegative side and retains the edge
		if( d0 >= 0.0f && d1 >= 0.0f ) {
			continue;
		}

		// the edge is split by the plane.  copmute the point of intersection.
		// if the old edge is <v0,v1> and currEdge is the intersection point,
		// the new edge is <v0,currEdge> when d0 > 0, or<currEdge,v1> when d1 > 0.
		const int vNew = _vertices.size();
		_vertices.push_back(CVertex());
		CVertex& vertexNew = _vertices[vNew];

		const cc::Vec3f p0 = _vertices[edge.vertex[0]].point;
		const cc::Vec3f p1 = _vertices[edge.vertex[1]].point;
		vertexNew.point = p0 + (d0/(d0 - d1))*(p1 - p0);

		if( d0 > 0.0f ) {
			edge.vertex[1] = vNew;
		} else {
			edge.vertex[0] = vNew;
		}
	}
}

void NewClipMesh::processFaces( const Plane& clippingPlane ) {
	// the mesh straddles the plane.  a new convex polygon face will be
	// generated.  add it now and insert edges when they are visited.
	const int fNew = _faces.size();
	_faces.push_back(CFace());
	CFace& faceNew = _faces[fNew];
	faceNew.normal = clippingPlane.normal;

	// process the faces
	for( unsigned int currFace = 0; currFace < fNew; ++currFace ) {
		CFace& face = _faces[currFace];
		if( !face.visible ) {
			continue;
		}

		// determine if the face is on the negative side, the positive side,
		// or split by the clipping plane.  the occurs members are set to zero
		// to help find the end points of the polyline that results from clipping
		// a face.
		assert(face.edges.size() >= 2 ); // unexpected condition
		std::set<int>::const_iterator iter = face.edges.begin();
		const std::set<int>::const_iterator end = face.edges.end();
		while( iter != end ) {
			const int e = *iter++;
			CEdge& edge = _edges[e];
			assert(edge.visible); // unexpected condition
			_vertices[edge.vertex[0]].occurs = 0;
			_vertices[edge.vertex[1]].occurs = 0;
		}

		int vStart;
		int vFinal;
		if( getOpenPolyline(face, vStart, vFinal) ) {
			// polyline is open, close it up
			const int eNew = _edges.size();
			_edges.push_back(CEdge());
			CEdge& edgeNew = _edges[eNew];

			edgeNew.vertex[0] = vStart;
			edgeNew.vertex[1] = vFinal;
			edgeNew.faces.insert(currFace);
			edgeNew.faces.insert(fNew);

			// add new edge to polygons
			face.edges.insert(eNew);
			faceNew.edges.insert(eNew);
		}
	}

	// process 'faceNew' to make sure it is a simple polygon (theoretically
	// convex, but numerically may be slightly not convex).  floating-point
	// round-off errors can cause the new face from the last loop to be
	// needle-like with a collapse of two edges into a single edge.  this
	// block guarantees the invariant face is always a simple polygon.
	postProcess(fNew, faceNew);
	if( faceNew.edges.size() < 3 ) {
		// face is completely degenerate, remote it from mesh
		_faces.pop_back();
	}
}

bool NewClipMesh::getOpenPolyline( CFace& face, int& vStart, int& vFinal ) {
	// count the number of occurrences of each vertex in the polyline
	bool okay = true;
	std::set<int>::const_iterator iter = face.edges.begin();
	std::set<int>::const_iterator end = face.edges.end();
	while( iter != end ) {
		const int e = *iter++;
		CEdge& edge = _edges[e];

		const int v0 = edge.vertex[0];
		++_vertices[v0].occurs;
		if( _vertices[v0].occurs > 2 ) {
			okay = false;
		}

		const int v1 = edge.vertex[1];
		++_vertices[v1].occurs;
		if( _vertices[v1].occurs > 2 ) {
			okay = false;
		}
	}

	if( !okay ) {
		// if you reach this block, there is a good chance that floating point
		// round-off error had caused this face to be needle-like and what was
		// theoretically a narrow V-shaped portion (a vertex shared by two edges
		// forming a small angle) has collapsed into a single line segment.
		// note that after adding post-processing, this hasn't happened.
		assert(false);
		// todo: print out error data for analysis
		return false;
	}

	iter = face.edges.begin();
	end = face.edges.end();
	vStart = -1;
	vFinal = -1;
	while( iter != end ) {
		const int e = *iter++;
		CEdge& edge = _edges[e];

		const int v0 = edge.vertex[0];
		if( 1 == _vertices[v0].occurs ) {
			if( -1 == vStart ) {
				vStart = v0;
			} else if( -1 == vFinal ) {
				vFinal = v0;
			} else {
				// if you reach this assert,  there is a good chance that the polyhedron
				// is not convex.
				assert(false); // polyhedron might not be convex
			}
		}

		const int v1 = edge.vertex[1];
		if( 1 == _vertices[v1].occurs ) {
			if( -1 == vStart ) {
				vStart = v1;
			} else if( -1 == vFinal ) {
				vFinal = v1;
			} else {
				// if you reach this assert, there is a good chance that the polyhedron
				// is not convex.
				assert(false); // polyhedron might not be convex
			}
		}
	}

	assert((-1==vStart && -1==vFinal) || (-1 != vStart && -1 != vFinal)); // unexpected condition
	return vStart != -1;
}

void NewClipMesh::postProcess( int fNew, CFace& faceNew ) {
	const int numEdges = faceNew.edges.size();
	std::vector<CEdgePlus> edges(numEdges);
	std::set<int>::const_iterator iter = faceNew.edges.begin();
	std::set<int>::const_iterator end = faceNew.edges.end();
	int counter = 0;
	while( iter != end ) {
		const int e = *iter++;
		edges[counter++] = CEdgePlus(e, _edges[e]);
	}
	std::sort(edges.begin(), edges.end());

	// process duplicate edges
	for( int i0=0, i1=1; i1 < numEdges; i0 = i1++) {
		if( edges[i0] == edges[i1] ) {
			const int i2 = i1 + 1;
			if( i2 < numEdges ) {
				// make sure an edge occurs at most twice.  if not, then the algorithm
				// needs to be modified to handle it.
				assert(edges[i1] != edges[i2]); // unexpected condition
			}

			// todo: figure out what the algorithm does

			// edge e0 has vertices v0,v1 and faces f0,nf.
			// edge e1 has vertices v0,v1 and faces f1,nf.

			// 1. get edges
			const int e0 = edges[i0].E;
			const int e1 = edges[i1].E;
			CEdge& edge0 = _edges[e0];
			CEdge& edge1 = _edges[e1];

			// 2. remove e0 and e1 from faceNew
			faceNew.edges.erase(e0);
			faceNew.edges.erase(e1);

			// 3. remove faceNew from e0
			edge0.faces.erase(fNew);

			// 4. remove faceNew from e1
			edge1.faces.erase(fNew);

			// 5. e1 is being booted from the system.  update the face f1 that shares it.
			// update e1 to share f1.
			iter = edge1.faces.begin();
			end = edge1.faces.end();
			while( iter != end ) {
				const int idx = *iter++;
				CFace& face = _faces[idx];
				face.edges.erase(e1);
				face.edges.insert(e0);
				edge0.faces.insert(idx);
				edge1.visible = false;
			}
		}
	}
}

void NewClipMesh::getTriangles( std::vector<int>& indices ) {
	const int numFaces = _faces.size();
	for( int currFace = 0; currFace < numFaces; ++currFace ) {
		CFace& face = _faces[currFace];
		if( !face.visible ) {
			continue;
		}

		const int numEdges = face.edges.size();
		assert(numEdges >= 3); // unexpected condition
		std::vector<int> vOrdered(numEdges+1);
		orderVertices(face, vOrdered);

		const int v0 = vOrdered[0];
		const int v2 = vOrdered[numEdges - 1];
		const int v1 = vOrdered[(numEdges - 1) >> 1];
		const cc::Vec3f diff1 = _vertices[v1].point - _vertices[v0].point;
		const cc::Vec3f diff2 = _vertices[v2].point - _vertices[v0].point;
		const float sgnVolume = face.normal.dot(diff1.cross(diff2));
		if( sgnVolume < 0.0f ) { // feel free to invert this test
			// clockwise, need to swap
			for( int i = 1; i + 1 < numEdges; ++i ) {
				indices.push_back(v0);
				indices.push_back(vOrdered[i + 1]);
				indices.push_back(vOrdered[i]);
			}
		} else {
			// counterclockwise
			for( int i = 1; i + 1 < numEdges; ++i ) {
				indices.push_back(v0);
				indices.push_back(vOrdered[i]);
				indices.push_back(vOrdered[i + 1]);
			}
		}
	}
}

void NewClipMesh::orderVertices( CFace& face, std::vector<int>& vOrdered ) {
	// copy edge indices into contiguous memory
	const int numEdges = face.edges.size();
	std::vector<int> eOrdered(numEdges);
	std::set<int>::const_iterator iter = face.edges.begin();
	std::set<int>::const_iterator end = face.edges.end();
	int counter = 0;
	while( iter != end ) {
		eOrdered[counter++] = *iter++;
	}

	std::sort(eOrdered.begin(), eOrdered.end());
	//// bubble sort
	//for( int i0 = 0, i1 = 1, choice = 1; i1 < numEdges - 1; i0 = i1++ ) {
	//	CEdge& edgeCurr = _edges[eOrdered[i0]];
	//	int j, curr = edgeCurr.vertex[choice];
	//	for( j = i1; j < numEdges; ++j ) {
	//		CEdge& edgeTemp = _edges[eOrdered[j]];
	//		int save;
	//		if( edgeTemp.vertex[0] == curr ) {
	//			save = eOrdered[i1];
	//			eOrdered[i1] = eOrdered[j];
	//			eOrdered[j] = save;
	//			choice = 1;
	//			break;
	//		}
	//		if( edgeTemp.vertex[1] == curr ) {
	//			save = eOrdered[i1];
	//			eOrdered[i1] = eOrdered[j];
	//			eOrdered[j] = save;
	//			choice = 0;
	//			break;
	//		}
	//	}
	//	assert(j < numEdges); // unexpected condition
	//}

	vOrdered[0] = _edges[eOrdered[0]].vertex[0];
	vOrdered[1] = _edges[eOrdered[0]].vertex[1];
	for( int i = 1; i < numEdges; ++i ) {
		CEdge& edge = _edges[eOrdered[i]];
		if( edge.vertex[0] == vOrdered[i] ) {
			vOrdered[i + 1] = edge.vertex[1];
		} else {
			vOrdered[i + 1] = edge.vertex[0];
		}
	}
}