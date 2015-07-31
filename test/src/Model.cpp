#include "Model.hpp"
#include <ciri/gfx/IVertexBuffer.hpp>
#include <ciri/gfx/IIndexBuffer.hpp>

Model::Model()
	: _vertexBuffer(nullptr), _indexBuffer(nullptr) {
}

Model::~Model() {
}

void Model::addVertex( const Vertex& vertex ) {
	_vertices.push_back(vertex);
}

void Model::addIndex( int index ){
	_indices.push_back(index);
}

bool Model::build( ciri::IGraphicsDevice* device ) {
	if( _vertexBuffer != nullptr ) {
		return false;
	}

	if( 0 ==_vertices.size() ) {
		return false;
	}

	_vertexBuffer = device->createVertexBuffer();
	if( !_vertexBuffer->set(_vertices.data(), sizeof(Vertex), _vertices.size(), false) ) {
		_vertexBuffer->destroy();
		delete _vertexBuffer;
		_vertexBuffer = nullptr;
		return false;
	}

	if( _indices.size() > 0 ) {
		_indexBuffer = device->createIndexBuffer();
		if( !_indexBuffer->set(_indices.data(), _indices.size(), false) ) {
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
