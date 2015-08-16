#include "Model.hpp"
#include <ciri/gfx/IVertexBuffer.hpp>
#include <ciri/gfx/IIndexBuffer.hpp>
#include <ciri/gfx/ObjModel.hpp>

Model::Model()
	: _vertexBuffer(nullptr), _indexBuffer(nullptr), _shader(nullptr) {
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

bool Model::build( ciri::IGraphicsDevice* device ) {
	if( _vertexBuffer != nullptr ) {
		return false;
	}

	if( 0 ==_vertices.size() ) {
		return false;
	}

	_vertexBuffer = device->createVertexBuffer();
	if( ciri::err::failed(_vertexBuffer->set(_vertices.data(), sizeof(Vertex), _vertices.size(), false)) ) {
		_vertexBuffer->destroy();
		delete _vertexBuffer;
		_vertexBuffer = nullptr;
		return false;
	}

	if( _indices.size() > 0 ) {
		_indexBuffer = device->createIndexBuffer();
		if( ciri::err::failed(_indexBuffer->set(_indices.data(), _indices.size(), false)) ) {
			_vertexBuffer->destroy();
			delete _vertexBuffer;
			_vertexBuffer = nullptr;
			_indexBuffer->destroy();
			delete _indexBuffer;
			_indexBuffer = nullptr;
			return false;
		}
	}

	return true;
}

ciri::IVertexBuffer* Model::getVertexBuffer() const {
	return _vertexBuffer;
}

ciri::IIndexBuffer* Model::getIndexBuffer() const {
	return _indexBuffer;
}

Transform& Model::getXform() {
	return _xform;
}

ciri::IShader* Model::getShader() const {
	return _shader;
}

void Model::setShader( ciri::IShader* val ) {
	_shader = val;
}