#include "Model.hpp"
#include <ciri/gfx/IVertexBuffer.hpp>
#include <ciri/gfx/IIndexBuffer.hpp>
#include <ciri/gfx/ObjModel.hpp>
#include <map>
#include <cc/TriMath.hpp>

Model::Model()
	: _vertexBuffer(nullptr), _indexBuffer(nullptr), _shader(nullptr), _dynamicVertex(false), _dynamicIndex(false) {
}

Model::~Model() {
}

void Model::addVertex( const Vertex& vertex ) {
	_vertices.push_back(vertex);
}

void Model::addIndex( int index ){
	_indices.push_back(index);
}

bool Model::addFromObj( const char* file, bool outputErrors ) {
	ciri::ObjModel obj;
	if( !obj.parse(file) ) {
		return false;
	}

	_vertices.clear();
	_indices.clear();

	const std::vector<cc::Vec3f>& positions = obj.getPositions();
	const std::vector<cc::Vec3f>& normals = obj.getNormals();
	const std::vector<cc::Vec2f>& texcoords = obj.getTexcoords();
	const std::vector<ciri::ObjModel::ObjVertex>& objVertices = obj.getVertices();

	//printf("pos: %d; nrm: %d; tex: %d; verts: %d\n", positions.size(), normals.size(), texcoords.size(), objVertices.size());

	if( objVertices.size() == 0 ) {
		return false;
	}

	for( unsigned int i = 0; i < objVertices.size(); ++i ) {
		Vertex vert;

		// get position
		const int posIdx = objVertices[i].position;
		if( -1 == posIdx ) {
			if( outputErrors ) {
				printf("Vertex %d's position index is -1.  Generating zero position.\n", i);
			}
			vert.position = cc::Vec3f::zero();
		} else if( posIdx >= positions.size() ) {
			printf("Vertex %d's position index is out of bounds (%d->%d).\n", i, posIdx, positions.size());
		} else {
			vert.position = positions[posIdx];
		}

		// get normal
		const int nrmIdx = objVertices[i].normal;
		if( -1 == nrmIdx ) {
			if( outputErrors ) {
				printf("Vertex %d's normal index is -1.  Generating zero normal.\n", i);
			}
			vert.normal = cc::Vec3f::zero();
		}
		else if( nrmIdx >= normals.size() ) {
			printf("Vertex %d's normal index is out of bounds (%d->%d).\n", i, nrmIdx, normals.size());
		} else {
			vert.normal = normals[nrmIdx];
		}

		// get texcoord
		const int texIdx = objVertices[i].texcoord;
		if( -1 == texIdx ) {
			if( outputErrors ) {
				printf("Vertex %d's texcoord index is -1.  Generating zero texcoord.\n", i);
			}
			vert.texcoord = cc::Vec2f::zero();
		} else if( texIdx >= texcoords.size() ) {
			printf("Vertex %d's texcoord index is out of bounds (%d->%d).\n", i, texIdx, texcoords.size());
		} else {
			vert.texcoord = texcoords[texIdx];
		}

		_vertices.push_back(vert);
	}

	return true;
}

bool Model::computeNormals() {
	const std::vector<Triangle> tris = getTriangles();
	for( int i = 0; i < tris.size(); ++i ) {
		const Triangle& currTri = tris[i];
		const cc::Vec3f normal = cc::math::computeTriangleNormal(_vertices[currTri.idx[0]].position, _vertices[currTri.idx[1]].position, _vertices[currTri.idx[2]].position).normalized();
		_vertices[currTri.idx[0]].normal += normal;
		_vertices[currTri.idx[1]].normal += normal;
		_vertices[currTri.idx[2]].normal += normal;
	}
	for( int i = 0; i < _vertices.size(); ++i ) {
		_vertices[i].normal.normalize();
	}

	return true;
}

bool Model::computeTangents() {
	// NOTE: assumes triangle indices

	// cannot compute tangents for an already generated model
	if( isValid() ) {
		return false;
	}

	// must have indices and vertices
	if( _indices.size() == 0 || _vertices.size() == 0 ) {
		return false;
	}

	std::vector<cc::Vec3f> tan1(_vertices.size());
	std::vector<cc::Vec3f> tan2(_vertices.size());
	const int triangleCount = _indices.size();

	for( int i = 0; i < triangleCount; i += 3 ) {
		// indices
		const int i1 = _indices[i+0];
		const int i2 = _indices[i+1];
		const int i3 = _indices[i+2];

		// positions of triangle
		const cc::Vec3f& v1 = _vertices[i1].position;
		const cc::Vec3f& v2 = _vertices[i2].position;
		const cc::Vec3f& v3 = _vertices[i3].position;
		// texcoords of triangle

		const cc::Vec2f& w1 = _vertices[i1].texcoord;
		const cc::Vec2f& w2 = _vertices[i2].texcoord;
		const cc::Vec2f& w3 = _vertices[i3].texcoord;
		// deltas
		const float x1 = v2.x - v1.x;
		const float x2 = v3.x - v1.x;
		const float y1 = v2.y - v1.y;
		const float y2 = v3.y - v1.y;
		const float z1 = v2.z - v1.z;
		const float z2 = v3.z - v1.z;
		const float s1 = w2.x - w1.x;
		const float s2 = w3.x - w1.x;
		const float t1 = w2.y - w1.y;
		const float t2 = w3.y - w1.y;

		// sdir & tdir
		const float r = 1.0f / (s1 * t2 - s2 * t1);
		const cc::Vec3f sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
		const cc::Vec3f tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);
		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;        
		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;
	}

	for( unsigned int i = 0; i < _vertices.size(); ++i ) {
		const cc::Vec3f& n = _vertices[i].normal;
		const cc::Vec3f& t = tan1[i];

		// Gram-Schmidt orthogonalize
		_vertices[i].tangent = (t - n * n.dot(t)).normalized();

		// Calculate handedness
		_vertices[i].tangent.w = (n.cross(t).dot(tan2[i]) < 0.0f) ? -1.0f : 1.0f;
	}

	return true;
}

bool Model::build( std::shared_ptr<ciri::IGraphicsDevice> device ) {
	if( _vertexBuffer != nullptr ) {
		return false;
	}

	if( 0 ==_vertices.size() ) {
		return false;
	}

	_vertexBuffer = device->createVertexBuffer();
	if( ciri::failed(_vertexBuffer->set(_vertices.data(), sizeof(Vertex), _vertices.size(), _dynamicVertex)) ) {
		_vertexBuffer->destroy();
		_vertexBuffer.reset();
		_vertexBuffer = nullptr;
		return false;
	}

	if( _indices.size() > 0 ) {
		_indexBuffer = device->createIndexBuffer();
		if( ciri::failed(_indexBuffer->set(_indices.data(), _indices.size(), _dynamicIndex)) ) {
			_vertexBuffer->destroy();
			_vertexBuffer.reset();
			_vertexBuffer = nullptr;
			_indexBuffer->destroy();
			_indexBuffer.reset();
			_indexBuffer = nullptr;
			return false;
		}
	}

	return true;
}

bool Model::updateBuffers( bool vertex, bool index ) {
	if( !isValid() ) {
		return false;
	}

	bool success = true;

	if( vertex ) {
		// cannot update non-dynamic buffer
		if( !_dynamicVertex ) {
			success = false;
		} else {
			if( ciri::failed(_vertexBuffer->set(_vertices.data(), sizeof(Vertex), _vertices.size(), true)) ) {
				success = false;
			}
		}
	}

	if( index ) {
		// cannot update non-dynamic buffer
		if( !_dynamicIndex ) {
			success = false;
		} else {
			if( ciri::failed(_indexBuffer->set(_indices.data(), _indices.size(), true)) ) {
				success = false;
			}
		}
	}

	return true;
}

bool Model::flipNormals( bool shouldUpdateBuffers ) {
	// check for request to update buffers with no valid bfufers
	if( shouldUpdateBuffers && !isValid() ) {
		return false;
	}

	for( auto vert : _vertices ) {
		vert.normal = -vert.normal;
	}

	if( shouldUpdateBuffers ) {
		return updateBuffers(true, false);
	}
	
	return true;
}

const std::shared_ptr<ciri::IVertexBuffer>& Model::getVertexBuffer() const {
	return _vertexBuffer;
}

const std::shared_ptr<ciri::IIndexBuffer>& Model::getIndexBuffer() const {
	return _indexBuffer;
}

Transform& Model::getXform() {
	return _xform;
}

const std::shared_ptr<ciri::IShader>& Model::getShader() const {
	return _shader;
}

void Model::setShader( const std::shared_ptr<ciri::IShader>& val ) {
	_shader = val;
}

void Model::setDynamicity( bool vertex, bool index ) {
	if( isValid() ) {
		return;
	}

	_dynamicVertex = vertex;
	_dynamicIndex = index;
}

bool Model::isValid() const {
	return _vertexBuffer != nullptr;
}

std::vector<Vertex>& Model::getVertices() {
	return _vertices;
}

std::vector<int>& Model::getIndices() {
	return _indices;
}

const std::vector<Model::Triangle>& Model::getTriangles() {
	if( 0 == _triangles.size() ) {
		parseExtendedData();
	}
	return _triangles;
}

const std::vector<Model::Edge>& Model::getEdges() {
	if( 0 == _edges.size() ) {
		parseExtendedData();
	}
	return _edges;
}

Model Model::copy() const {
	Model mdl;
	mdl._vertices = _vertices;
	mdl._indices = _indices;
	mdl._vertexBuffer = _vertexBuffer;
	mdl._indexBuffer =_indexBuffer;
	mdl._xform = _xform;
	mdl._shader = _shader;
	mdl._dynamicVertex = _dynamicVertex;
	mdl._dynamicIndex = _dynamicIndex;
	return mdl;
}

bool Model::parseExtendedData() {
	// requires triangles
	if( (_indices.size() % 3) != 0 ) {
		return false;
	}

	//
	// clear any old data
	//
	_triangles.clear();
	_edges.clear();

	//
	// gather all triangles
	//
	for( int i = 0; i < _indices.size(); i += 3 ) {
		Triangle t;
		t.idx[0] = _indices[i];
		t.idx[1] = _indices[i+1];
		t.idx[2] = _indices[i+2];
		_triangles.push_back(t);
	}

	//
	// gather all edges
	//
//#define MODEL_UNIQUE_EDGES 1
#ifndef MODEL_UNIQUE_EDGES
	for( int i = 0; i < _triangles.size(); ++i ) {
		const Model::Triangle& t = _triangles[i];
		Edge e0, e1, e2;
		e0.idx[0] = t.idx[0];
		e0.idx[1] = t.idx[1];
		e1.idx[0] = t.idx[1];
		e1.idx[1] = t.idx[2];
		e2.idx[0] = t.idx[2];
		e2.idx[1] = t.idx[0];
		e0.faces.push_back(i);
		e1.faces.push_back(i);
		e2.faces.push_back(i);
		_edges.push_back(e0);
		_edges.push_back(e1);
		_edges.push_back(e2);
	}
#else
	// use map and reverse index checking to ensure only unique edges are stored and appended
	std::map<std::pair<int, int>, int> edgeMap;
	std::vector<Model::Triangle> triangles = getTriangles();
	for( int i = 0; i < triangles.size(); ++i ) {
		const Model::Triangle& currTri = triangles[i];
		// edge 0->1
		std::pair<int, int> e01(currTri.idx[0], currTri.idx[1]);
		std::pair<int, int> e02(currTri.idx[1], currTri.idx[0]);
		if( edgeMap.find(e02) != edgeMap.end() ) { // if has reversed...
			_edges[edgeMap[e02]].faces.push_back(i); // add current face to list
		} else if( edgeMap.find(e01) != edgeMap.end() ) { // if has normal...
			_edges[edgeMap[e01]].faces.push_back(i); // add current face to lsit
		} else {
			Edge e;
			e.idx[0] = e01.first;
			e.idx[1] = e01.second;
			e.faces.push_back(i);
			_edges.push_back(e);
			edgeMap[e01] = _edges.size()-1;
		}
		// edge 1->2
		std::pair<int, int> e11(currTri.idx[1], currTri.idx[2]);
		std::pair<int, int> e12(currTri.idx[2], currTri.idx[1]);
		if( edgeMap.find(e12) != edgeMap.end() ) {
			_edges[edgeMap[e12]].faces.push_back(i);
		} else if( edgeMap.find(e11) != edgeMap.end() ) {
			_edges[edgeMap[e11]].faces.push_back(i);
		} else {
			Edge e;
			e.idx[0] = e11.first;
			e.idx[1] = e11.second;
			e.faces.push_back(i);
			_edges.push_back(e);
			edgeMap[e11] = _edges.size()-1;
		}
		// edge 2->0
		std::pair<int, int> e21(currTri.idx[2], currTri.idx[0]);
		std::pair<int, int> e22(currTri.idx[0], currTri.idx[2]);
		if( edgeMap.find(e22) != edgeMap.end() ) {
			_edges[edgeMap[e22]].faces.push_back(i);
		} else if( edgeMap.find(e21) != edgeMap.end() ) {
			_edges[edgeMap[e21]].faces.push_back(i);
		} else {
			Edge e;
			e.idx[0] = e21.first;
			e.idx[1] = e21.second;
			e.faces.push_back(i);
			_edges.push_back(e);
			edgeMap[e21] = _edges.size()-1;
		}
	}
#endif

	//
	// link triangles' edges (edges are already linked to triangles)
	//
	for( int i = 0; i < _edges.size(); ++i ) {
		const Edge& currEdge = _edges[i];
		for( int j = 0; j < currEdge.faces.size(); ++j ) {
			_triangles[currEdge.faces[j]].edges.push_back(i);
		}
	}
}