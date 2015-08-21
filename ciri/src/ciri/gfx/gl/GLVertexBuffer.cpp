#include <ciri/gfx/gl/GLVertexBuffer.hpp>

namespace ciri {
	GLVertexBuffer::GLVertexBuffer()
		: IVertexBuffer(), _vbo(0), _vertexStride(0), _vertexCount(0), _isDynamic(false) {
	}

	GLVertexBuffer::~GLVertexBuffer() {
		destroy();
	}

	err::ErrorCode GLVertexBuffer::set( void* vertices, int vertexStride, int vertexCount, bool dynamic ) {
		if( nullptr == vertices || vertexStride <= 0 || vertexCount <= 0 ) {
			return err::CIRI_INVALID_ARGUMENT;
		}

		if( _vbo != 0 ) {
			// cannot update a static vertex buffer
			if( !_isDynamic ) {
				return err::CIRI_STATIC_BUFFER_AS_DYNAMIC;
			}

			// for now, size must be the same as the original data
			if( vertexStride != _vertexStride || vertexCount != _vertexCount ) {
				return err::CIRI_NOT_IMPLEMENTED; // todo
			}

			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, vertexStride * vertexCount, vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			return err::CIRI_OK;
		}

		_vertexStride = vertexStride;
		_vertexCount = vertexCount;
		_isDynamic = dynamic;

		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexStride * vertexCount, vertices, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// todo: check for fail

		return err::CIRI_OK;
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
} // ciri