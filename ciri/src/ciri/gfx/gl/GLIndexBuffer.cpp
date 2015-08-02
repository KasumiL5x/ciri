#include <ciri/gfx/gl/GLIndexBuffer.hpp>

namespace ciri {
	GLIndexBuffer::GLIndexBuffer()
		: IIndexBuffer(), _evbo(0), _indexCount(0) {
	}

	GLIndexBuffer::~GLIndexBuffer() {
		destroy();
	}

	err::ErrorCode GLIndexBuffer::set( int* indices, int indexCount, bool dynamic ) {
		if( nullptr == indices || indexCount <= 0 ) {
			return err::CIRI_INVALID_ARGUMENT;
		}

		// todo: add remapping (updating) support
		if( _evbo != 0 ) {
			return err::CIRI_NOT_IMPLEMENTED;
		}

		_indexCount = indexCount;

		glGenBuffers(1, &_evbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _evbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexCount, indices, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// todo: check for fail

		return err::CIRI_OK;
	}

	void GLIndexBuffer::destroy() {
		if( _evbo != 0 ) {
			glDeleteBuffers(1, &_evbo);
			_evbo = 0;
		}
	}

	int GLIndexBuffer::getIndexCount() const {
		return _indexCount;
	}

	GLuint GLIndexBuffer::getEvbo() const {
		return _evbo;
	}
} // ciri