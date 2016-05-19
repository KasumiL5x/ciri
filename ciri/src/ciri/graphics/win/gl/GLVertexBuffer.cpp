#include <ciri/graphics/win/gl/GLVertexBuffer.hpp>

using namespace ciri;

GLVertexBuffer::GLVertexBuffer()
	: IVertexBuffer(), _vbo(0), _vertexStride(0), _vertexCount(0), _isDynamic(false) {
}

GLVertexBuffer::~GLVertexBuffer() {
	destroy();
}

ErrorCode GLVertexBuffer::set( void* vertices, int vertexStride, int vertexCount, bool dynamic ) {
	if( nullptr == vertices || vertexStride <= 0 || vertexCount <= 0 ) {
		return ErrorCode::CIRI_INVALID_ARGUMENT;
	}

	// update if already valid
	if( _vbo != 0 ) {
		// cannot update a static vertex buffer
		if( !_isDynamic ) {
			return ErrorCode::CIRI_STATIC_BUFFER_AS_DYNAMIC;
		}

		// do not let the vertex stride change regardless until i decide how to handle it
		if( _vertexStride != vertexStride ) {
			return ErrorCode::CIRI_NOT_IMPLEMENTED;
		}

		// if the new data is larger...
		if( (vertexStride * vertexCount) > (_vertexStride * _vertexCount) ) {
			destroy();
			return createBuffer(vertices, vertexStride, vertexCount, true);
		}

		return updateBuffer(vertices, vertexStride, vertexCount);
	}

	return createBuffer(vertices, vertexStride, vertexCount, dynamic);
}

void GLVertexBuffer::destroy() {
	if( _vbo != 0 ) {
		glDeleteBuffers(1, &_vbo);
		_vbo = 0;
	}
}

int GLVertexBuffer::getStride() const {
	return _vertexStride;
}

int GLVertexBuffer::getVertexCount() {
	return _vertexCount;
}

GLuint GLVertexBuffer::getVbo() const {
	return _vbo;
}

ErrorCode GLVertexBuffer::createBuffer( void* vertices, int vertexStride, int vertexCount, bool dynamic ) {
	// generate a new buffer, bind it, set the data, and unbind it
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexStride * vertexCount, vertices, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// store settings upon successful buffer creation
	_vertexStride = vertexStride;
	_vertexCount = vertexCount;
	_isDynamic = dynamic;

	return ErrorCode::CIRI_OK;
}

ErrorCode GLVertexBuffer::updateBuffer( void* vertices, int vertexStride, int vertexCount ) {
	// bind, upload new data, and unbind
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertexStride * vertexCount, vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return ErrorCode::CIRI_OK;
}