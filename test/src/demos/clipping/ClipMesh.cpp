#include "ClipMesh.hpp"
#include <cc/TriMath.hpp>
#include <algorithm>

// notes: seems to generate a somewhat usable clip (and fills the hole), although there's some bugs.
//        an index in my cube test seems to be minus 3 million or so, but only one!  not sure why yet.
//        i'm going to rewrite this to copy the ConvexClipper.cpp style completely, and then tweak from there.
//        all in all, good progress so far.
//gionrgoijrgiorgijog

ClipMesh::ClipMesh( Model& source ) {
	// copy vertices over
	const std::vector<Vertex>& srcVertices = source.getVertices();
	V.resize(srcVertices.size());
	for( int i = 0; i < srcVertices.size(); ++i ) {
		V[i].point = srcVertices[i].position;
	}

	// copy edges over
	std::vector<Model::Edge> srcEdges = source.getEdges();
	E.resize(srcEdges.size());
	for( int i = 0; i < srcEdges.size(); ++i ) {
		E[i].vertex[0] = srcEdges[i].idx[0];
		E[i].vertex[1] = srcEdges[i].idx[1];
		for( int j = 0; j < srcEdges[i].faces.size(); ++j ) {
			E[i].face.push_back(srcEdges[i].faces[j]);
		}
	}

	// copy faces over
	std::vector<Model::Triangle> srcTriangles = source.getTriangles();
	F.resize(srcTriangles.size());
	for( int i = 0; i < srcTriangles.size(); ++i ) {
		const cc::Vec3f& p0 = srcVertices[srcTriangles[i].idx[0]].position;
		const cc::Vec3f& p1 = srcVertices[srcTriangles[i].idx[1]].position;
		const cc::Vec3f& p2 = srcVertices[srcTriangles[i].idx[2]].position;
		F[i].plane = cc::math::computeTriangleNormal(p0, p1, p2).normalized();
		for( int j = 0; j < srcTriangles[i].edges.size(); ++j ) {
			F[i].edge.insert(srcTriangles[i].edges[j]);
		}
	}
}

int ClipMesh::clip( const Plane& clipPlane ) {
	// compute distanecs to plane, set visibility flags
	const int vertexResult = processVertices(clipPlane);

	// if all vertices are on nonnegative side of the plane (no clipping occurs)
	if( 1 == vertexResult ) {
		return 1;
	}

	// if all vertices are on nonpositive side of the plane (everything is clipped)
	if( -1 == vertexResult ) {
		return -1;
	}

	// mesh straddles the plane; some on positive side, some on negative side
	processEdges();
	processFaces(clipPlane);

	 return 0;
}

void ClipMesh::printDebug( bool verbose ) {
	int visibleVertices = 0;
	for( int i=0; i<V.size(); ++i){if(V[i].visible){visibleVertices+=1;}}
	printf("CVertex(%d, %d visible)\n", V.size(), visibleVertices);
	if( verbose ) {
		for( int i = 0; i < V.size(); ++i ) {
			printf("\t[%d] (%f, %f, %f); visible: %d\n", i, V[i].point.x, V[i].point.y, V[i].point.z, V[i].visible);
		}
	}

	int visibleEdges = 0;
	for( int i=0; i<E.size(); ++i){if(E[i].visible){visibleEdges+=1;}}
	printf("CEdge(%d, %d visible)\n", E.size(), visibleEdges);
	if( verbose ) {
		for( int i = 0; i < E.size(); ++i ) {
			printf("\t[%d] (%d, %d) connected to %d faces; visible: %d\n", i, E[i].vertex[0], E[i].vertex[1], E[i].face.size(), E[i].visible);
		}
	}

	int visibleFaces = 0;
	for( int i=0; i<F.size(); ++i){if(F[i].visible){visibleFaces+=1;}}
	printf("CFace(%d, %d visible)\n", F.size(), visibleFaces);
	if( verbose ) {
		for( int i = 0; i < F.size(); ++i ) {
			printf("\t[%d] has %d edges; visible: %d\n", i, F[i].edge.size(), F[i].visible);
		}
	}
}

Model ClipMesh::convert() {
	// create a lookup table for the visible vertices
	std::vector<Vertex> point;
	point.reserve(V.size());
	//std::unordered_map<int, int> vmap;//(V.size());
	std::vector<int> vmap(V.size());

	// copy visible vertices into the table
	for( int i = 0; i < V.size(); ++i ) {
		if( !V[i].visible ) {
			continue;
		}
		vmap[i] = point.size();
		point.push_back(Vertex(V[i].point, cc::Vec3f::zero(), cc::Vec2f::zero()));
	}

	// note: i have checked data input on a plane and it seems correct.
	//       the above vertices also seem correct for the plane.  the
	//       next function seems to be wrong, though.  it shouldn't return
	//       eight elements with a plane; it should be 6.

	// order the vertices for all the faces.  the output array has a sequence
	// of subarrays, each subarray having first element storing the number of
	// vertices in the face, the remaining elements storing the vertex indices
	// for that face in the correct order.  the indices are relative to V[].
	std::vector<int> faces = getOrderedFaces();

	// for some reason the last vertex is wrong (getting 303 instead of 301 etc.)
	//grniogriogfriomgrimo

	// reorder the indices
	for( int c = 0; c < faces.size(); ++c ) {
		const int oldC = faces[c];
		const int newC = vmap[oldC];
		faces[c] = newC;
	}

	// map the vertex indices to those of the new table
	//for( int i = 0; i < faces.size(); ) {
	//	const int numIndices = faces[i];
	//	i++;
	//	for( int j = 0; j < numIndices; ++j ) {
	//		faces[i] = vmap[faces[i]];
	//		i++;
	//	}
	//}

	Model mdl;
	// copy vertices to model
	for( int i = 0; i < point.size(); ++i ) {
		mdl.addVertex(point[i]);
	}
	// copy indices to model
	for( int i = 0; i < faces.size(); ++i ) {
		mdl.addIndex(faces[i]);
	}
	return mdl;
}

Model ClipMesh::convert2() {
	const int numVertices = V.size();
	std::vector<Vertex> points;
	int* vMap = new int[numVertices];
	memset(vMap, 0xFF, numVertices * sizeof(int));
	for( int v = 0; v < numVertices; ++v ) {
		const CVertex& vtx = V[v];
		if( !vtx.visible ) {
			continue;
		}
		vMap[v] = points.size();
		points.push_back(Vertex(vtx.point, cc::Vec3f(), cc::Vec2f()));
	}

	std::vector<int> indices;
	getTriangles(indices);

	for( int c = 0; c < indices.size(); ++c ) {
		const int oldC = indices[c];
		int newC = vMap[oldC];
		indices[c] = newC;
	}

	delete[] vMap; vMap = nullptr;

	Model mdl;
	for( int i = 0; i < points.size(); ++i ) {
		mdl.addVertex(points[i]);
	}
	for( int i = 0; i < indices.size(); ++i ) {
		mdl.addIndex(indices[i]);
	}

	return mdl;
}

int ClipMesh::processVertices( const Plane& clipPlane ) {
	// compute signed distances from vertices to plane
	int positive = 0;
	int negative = 0;
	for( int i = 0; i < V.size(); ++i ) {
		CVertex& vtx = V[i];
		if( !vtx.visible ) {
			continue;
		}

		vtx.distance = sqrDistPointPlane(clipPlane, vtx.point);
		if( vtx.distance >= 0.00001f ) {
			positive++;
		} else if( vtx.distance <= -0.00001f ) {
			negative++;
			vtx.visible = false;
		} else {
			// point on the plane within floating point tolerance
			vtx.distance = 0.0f;
		}
	}

	// all vertices on nonnegative side, no clipping
	if( 0 == negative ) {
		return 1;
	}

	// all vertices on nonpositive side; everything clipped
	if( 0 == positive ) {
		return -1;
	}

	return 0;
}

void ClipMesh::processEdges() {
	for( int i = 0; i < E.size(); ++i ) {
		CEdge& edge = E[i];
		if( !edge.visible ) {
			continue;
		}

		const float d0 = V[edge.vertex[0]].distance;
		const float d1 = V[edge.vertex[1]].distance;
		if( d0 <= 0.0f && d1 <= 0.0f ) {
			// edge is culled, remove edge from faces sharing it
			for( int j = 0; j < edge.face.size(); ++j ) {
				CFace& currFace = F[edge.face[j]];
				// remove edge
				currFace.edge.erase(i);
				// make invisibubble
				if( currFace.edge.empty() ) {
					currFace.visible = false;
				}
				//break;
				// todo: erase from edge too
			}
			//for( int j = 0; j < edge.face.size(); ++j ) {
			//	CFace& currFace = F[edge.face[j]];
			//	// remove specific edge using find
			//	for( int currEdge = 0; currEdge < F[edge.face[j]].edge.size(); ++currEdge ) {
			//		if( !(E[F[edge.face[j]].edge[currEdge]] == edge) ) {
			//			continue;
			//		}
			//		F[edge.face[j]].edge.erase(F[edge.face[j]].edge.begin() + currEdge);
			//		break;
			//	}

			//	if( F[edge.face[j]].edge.empty() ) {
			//		F[edge.face[j]].visible = false;
			//	}
			//}
			edge.visible = false;
			continue;
		}

		// edge is on nonnegative side, faces retain the edge
		if( d0 >= 0.0f && d1 >= 0.0f ) {
			continue;
		}
		
		const int vNew = V.size();
		V.push_back(CVertex());
		CVertex& vertexNew = V[vNew];
		const cc::Vec3f& point0 = V[edge.vertex[0]].point;
		const cc::Vec3f& point1 = V[edge.vertex[1]].point;
		vertexNew.point = point0 + (d0 / (d0 - d1))*(point1 - point0);
		if( d0 > 0.0f ) {
			edge.vertex[1] = vNew;
		} else {
			edge.vertex[0] = vNew;
		}

		// the edge is split by the plane.  compute the point of intersection.
		// if the old edge is <v0, v1>and i is the intersection point, the new
		// edge is <v0, i> when d0 > 0 or <i, v1> when d1 > 0.
		//const float t = d0 / (d0 - d1);
		//const cc::Vec3f intersect = (1.0f - t) * V[edge.vertex[0]].point + t * V[edge.vertex[1]].point;
		//const int index = V.size();
		//CVertex intersectVertex;
		//intersectVertex.point = intersect;
		//V.push_back(intersectVertex);

		//if( d0 > 0.0f ) {
		//	edge.vertex[1] = index;
		//} else {
		//	edge.vertex[0] = index;
		//}
	}
}

void ClipMesh::processFaces( const Plane& clipPlane ) {
	const int fNew = F.size();
	F.push_back(CFace());
	CFace& faceNew = F[fNew];
	faceNew.plane = clipPlane.normal;

	for( int currFace = 0; currFace < fNew; ++currFace ) {
		CFace& face = F[currFace];
		if( !face.visible ) {
			continue;
		}

		std::set<int>::iterator iter = face.edge.begin();
		std::set<int>::iterator end = face.edge.end();
		while( iter != end ) {
			const int e = *iter++;
			CEdge& edge = E[e];
			V[edge.vertex[0]].occurs = 0;
			V[edge.vertex[1]].occurs = 0;
		}

		int start, final;
		if( getOpenPolyline(face, start, final) ) {
			const int eNew = E.size();
			E.push_back(CEdge());
			CEdge& edgeNew = E[eNew];

			edgeNew.vertex[0] = start;
			edgeNew.vertex[1] = final;
			edgeNew.face.push_back(currFace);
			edgeNew.face.push_back(fNew);

			face.edge.insert(eNew);
			faceNew.edge.insert(eNew);
		}
	}

	if( faceNew.edge.size() < 3 ) {
		F.pop_back();
	}

	////const int findex = F.size();
	////CFace tmpCloseFace;
	////tmpCloseFace.plane = clipPlane.normal;
	////F.push_back(tmpCloseFace);
	////CFace* closeFace = &F[F.size()-1]; // ptr to tmpCloseFace

	//for( int i = 0; i < F.size(); ++i ) {
	//	if( !F[i].visible ) {
	//		continue;
	//	}

	//	// the edge is culled.  if the edge is exactly on the clip plane,
	//	// it is possible that a visible triangle shares it.
	//	// the edge will be re-added during the face loop
	//	for( int j = 0; j < F[i].edge.size(); ++j ) {
	//		V[E[j].vertex[0]].occurs = 0;
	//		V[E[j].vertex[1]].occurs = 0;
	//	}

	//	int start, final;
	//	if( getOpenPolyline(F[i], start, final) ) {
	//		// polyline is open, close it
	//		int eIndex = E.size();
	//		F[i].edge.insert(eIndex);
	//		//closeFace->edge.push_back(eIndex);

	//		CEdge closeEdge;
	//		closeEdge.vertex[0] = start;
	//		closeEdge.vertex[1] = final;
	//		closeEdge.face.push_back(i);
	//		//closeEdge.face.push_back(findex);
	//		E.push_back(closeEdge);
	//	}

	//	//int start, final;
	//	//if( getOpenPolyline(F[i], start, final) ) {
	//	//	// polyline is open, close it
	//	//	int eIndex = E.size();

	//	//	CEdge closeEdge;
	//	//	closeEdge.vertex[0] = start;
	//	//	closeEdge.vertex[1] = final;
	//	//	closeEdge.face.push_back(i);
	//	//	E.push_back(closeEdge);

	//	//	F[i].edge.push_back(eIndex);
	//	//}
	//}
}

bool ClipMesh::getOpenPolyline( CFace f, int& start, int& final ) {
	bool okay = true;
	std::set<int>::iterator iter = f.edge.begin();
	std::set<int>::iterator end = f.edge.end();
	while( iter != end ) {
		const int e = *iter++;
		CEdge& edge = E[e];

		const int v0 = edge.vertex[0];
		++V[v0].occurs;
		if( V[v0].occurs > 2 ) {
			okay = false;
		}

		const int v1 = edge.vertex[1];
		++V[v1].occurs;
		if( V[v1].occurs > 2 ) {
			okay = false;
		}
	}

	if( !okay ) {
		throw;
	}

	iter = f.edge.begin();
	end = f.edge.end();
	start = -1;
	final = -1;
	while( iter != end ) {
		const int e = *iter++;
		CEdge& edge = E[e];

		const int v0 = edge.vertex[0];
		if( 1 == V[0].occurs ) {
			if( -1 == start ) {
				start = v0;
			} else if( -1 == final ) {
				final = v0;
			} else {
				//throw; // error
			}
		}

		const int v1 = edge.vertex[1];
		if( 1 == V[v1].occurs ) {
			if( -1 == start ) {
				start = v1;
			} else if( -1 == final ) {
				final = v1;
			} else {
				//throw; // error
			}
		}
	}

	return start != -1;

	//// count the number of occurrences of each vertex in the polyline.
	//// resulting 'occurs' values must be 1 or 2
	//for( int j = 0; j < f.edge.size(); ++j ) {
	//	V[E[f.edge[j]].vertex[0]].occurs++;
	//	V[E[f.edge[j]].vertex[1]].occurs++;
	//}

	//// determine if the polyline is open
	//start = -1;
	//final = -1;
	//for( int j = 0; j < f.edge.size(); ++j ) {
	//	const int i0 = E[f.edge[j]].vertex[0];
	//	const int i1 = E[f.edge[j]].vertex[1];
	//	if( 1 == V[i0].occurs ) {
	//		if( -1 == start ) {
	//			start = i0;
	//		} else if( -1 == final ) {
	//			final = i0;
	//		}
	//	}
	//	if( 1 == V[i1].occurs ) {
	//		if( -1 == start ) {
	//			start = i1;
	//		} else if( -1 == final ) {
	//			final = i1;
	//		}
	//	}
	//}
	//return start != -1;
}

std::vector<int> ClipMesh::getOrderedFaces() {
	std::vector<int> faces;
	for( int i = 0; i < F.size(); ++i ) {
		if( !F[i].visible ) {
			continue;
		}

		const int numEdges = F[i].edge.size();
		const std::vector<int> vertices = getOrderedVertices(F[i]);

		const int v0 = vertices[0];
		const int v1 = vertices[(numEdges-1)>>1];
		const int v2 = vertices[numEdges-1];
		const cc::Vec3f diff1 = V[v1].point - V[v0].point;
		const cc::Vec3f diff2 = V[v2].point - V[v0].point;
		const float dp = F[i].plane.dot(diff1.cross(diff2));
		if( dp < 0.0f ) {
			for( int i = 1; i+1 < numEdges; ++i ) {
				faces.push_back(v0);
				faces.push_back(vertices[i+1]);
				faces.push_back(vertices[i]);
			}
		} else {
			for( int i = 1; i+1 < numEdges; ++i ) {
				faces.push_back(v0);
				faces.push_back(vertices[i]);
				faces.push_back(vertices[i+1]);
			}
		}
	}
	return faces;

	//std::vector<int> faces;
	//for( int i = 0; i < F.size(); ++i ) {
	//	if( !F[i].visible ) {
	//		continue;
	//	}

	//	// get the ordered vertices of the face.  the frist and last element
	//	// of the array are the same since the polyline is closed.
	//	const std::vector<int> vertices = getOrderedVertices(F[i]);
	//	faces.push_back(vertices.size()-1);

	//	// the convention is that the vertices should be ccw ordered when viewed
	//	// from the negative side of the plane of the face.  if you need the
	//	// opposite convention, switch the inequality in the if-else statement.
	//	if( F[i].plane.dot(getNormal(vertices)) > 0.0f ) {
	//		// clockwise, need to swap
	//		for( int j = vertices.size()-2; j >= 0; --j ) {
	//			faces.push_back(vertices[j]);
	//		}
	//	} else {
	//		// counterclockwise
	//		for( int j = 0; j <= vertices.size()-2; ++j ) {
	//			faces.push_back(vertices[j]);
	//		}
	//	}
	//}
	//return faces;
}

std::vector<int> ClipMesh::getOrderedVertices( CFace f ) {
	// copy edge indices into contiguous memory
	const int numEdges = f.edge.size();
	std::vector<int> eOrdered(numEdges);
	std::set<int>::iterator iter = f.edge.begin();
	std::set<int>::iterator end = f.edge.end();
	int counter = 0;
	while( iter != end ) {
		eOrdered[counter++] = *iter++;
	}

	// sort
	std::sort(eOrdered.begin(), eOrdered.end());

	// 
	std::vector<int> vOrdered(numEdges+1);
	vOrdered[0] = E[eOrdered[0]].vertex[0];
	vOrdered[1] = E[eOrdered[0]].vertex[1];
	for( int i = 1; i < numEdges; ++i ) {
		CEdge& edge = E[eOrdered[i]];
		if( edge.vertex[0] == vOrdered[i] ) {
			vOrdered[i+1] = edge.vertex[1];
		} else {
			vOrdered[i+1] = edge.vertex[0];
		}
	}
	return vOrdered;

	//// copy edge indices into contiguous memory for sorting
	//std::vector<int> edges;
	//edges.resize(f.edge.size());
	//int counter = 0;
	//for( int j = 0; j < f.edge.size(); ++j ) {
	//	edges[counter++] = f.edge[j];
	//}

	//std::sort(edges.begin(), edges.end());

	//std::vector<int> vertices;
	//vertices.resize(edges.size()+1);

	//// add the first two vertices
	//vertices[0] = E[edges[0]].vertex[0];
	//vertices[1] = E[edges[0]].vertex[1];

	//// add the remaining vertices
	//for( int i = 1; i < f.edge.size(); ++i ) {
	//	if( E[edges[i]].vertex[0] == vertices[i] ) {
	//		vertices[i+1] = E[edges[i]].vertex[1];
	//	} else {
	//		vertices[i+1] = E[edges[i]].vertex[0];
	//	}
	//}

	//return vertices;
}

cc::Vec3f ClipMesh::getNormal( const std::vector<int>& vertices ) const {
	cc::Vec3f normal(0.0f, 0.0f, 0.0f);
	for( int i = 0; i <= vertices.size()-2; ++i ) {
		normal += V[vertices[i]].point.cross(V[vertices[i+1]].point);
	}
	normal.normalize();
	return normal;
}

void ClipMesh::swap( int a, int b ) {
	CEdge tmp = E[a];
	E[a] = E[b];
	E[b] = tmp;
}

void ClipMesh::getTriangles( std::vector<int>& indices ) {
	const int numFaces = F.size();
	for( int f = 0; f < numFaces; ++f ) {
		CFace& face = F[f];
		if( !face.visible ) {
			continue;
		}

		const int numEdges = face.edge.size();
		std::vector<int> vOrdered(numEdges+1);
		orderVertices(face, vOrdered);

		const int v0 = vOrdered[0];
		const int v2 = vOrdered[numEdges - 1];
		const int v1 = vOrdered[(numEdges - 1) >> 1];
		const cc::Vec3f diff1 = V[v1].point - V[v0].point;
		const cc::Vec3f diff2 = V[v2].point - V[v0].point;
		const float sgnVolume = face.plane.dot(diff1.cross(diff2));
		if( sgnVolume > 0.0f ) {
			for (int i = 1; i + 1 < numEdges; ++i) {
				indices.push_back(v0);
				indices.push_back(vOrdered[i + 1]);
				indices.push_back(vOrdered[i]);
			}
		} else {
			for (int i = 1; i + 1 < numEdges; ++i) {
				indices.push_back(v0);
				indices.push_back(vOrdered[i]);
				indices.push_back(vOrdered[i + 1]);
			}
		}
	}
}

void ClipMesh::orderVertices( CFace& face, std::vector<int>& vOrdered ) {
	const int numEdges = (int)face.edge.size();
	std::vector<int> eOrdered(numEdges);
	std::set<int>::iterator iter = face.edge.begin();
	std::set<int>::iterator end = face.edge.end();
	int i = 0;
	while (iter != end)
	{
		eOrdered[i++] = *iter++;
	}

	for (int i0 = 0, i1 = 1, choice = 1; i1 < numEdges - 1; i0 = i1++)
	{
		CEdge& edgeCurr = E[eOrdered[i0]];
		int j, curr = edgeCurr.vertex[choice];
		for (j = i1; j < numEdges; ++j) {
			CEdge& edgeTemp = E[eOrdered[j]];
			int save;
			if (edgeTemp.vertex[0] == curr) {
				save = eOrdered[i1];
				eOrdered[i1] = eOrdered[j];
				eOrdered[j] = save;
				choice = 1;
				break;
			}
			if (edgeTemp.vertex[1] == curr) {
				save = eOrdered[i1];
				eOrdered[i1] = eOrdered[j];
				eOrdered[j] = save;
				choice = 0;
				break;
			}
		}
	}

	vOrdered[0] = E[eOrdered[0]].vertex[0];
	vOrdered[1] = E[eOrdered[0]].vertex[1];
	for (i = 1; i < numEdges; ++i) {
		CEdge& edge = E[eOrdered[i]];
		if (edge.vertex[0] == vOrdered[i]) {
			vOrdered[i + 1] = edge.vertex[1];
		} else {
			vOrdered[i + 1] = edge.vertex[0];
		}
	}
}