#include "Model.hpp"
#include <ciri/gfx/IVertexBuffer.hpp>
#include <ciri/gfx/IIndexBuffer.hpp>
#include <ciri/gfx/ObjModel.hpp>

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

bool Model::addFromObj( const char* file ) {
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

	if( objVertices.size() == 0 ) {
		return false;
	}

	for( unsigned int i = 0; i < objVertices.size(); ++i ) {
		Vertex vert;
		vert.position = positions[objVertices[i].position];
		vert.normal = normals[objVertices[i].normal];
		vert.texcoord = texcoords[objVertices[i].texcoord];
		_vertices.push_back(vert);
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