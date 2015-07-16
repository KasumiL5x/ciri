#include <ciri/gfx/gl/GLVertexBuffer.hpp>

namespace ciri {
	GLVertexBuffer::GLVertexBuffer( GLGraphicsDevice* device )
		: IVertexBuffer(), _device(device), _vbo(0), _vertexStride(0) {
	}

	GLVertexBuffer::~GLVertexBuffer() {
		destroy();
	}

	bool GLVertexBuffer::set( void* vertices, int vertexStride, int vertexCount, bool dynamic ) {
		// todo: add remapping (updating) support
		if( _vbo != 0 ) {
			return false;
		}

		_vertexStride = vertexStride;

		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexStride * vertexCount, vertices, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// todo: check for fail

		return true;
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

	GLuint GLVertexBuffer::getVbo() const {
		return _vbo;
	}
} // ciri