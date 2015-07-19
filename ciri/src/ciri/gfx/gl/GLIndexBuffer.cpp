#include <ciri/gfx/gl/GLIndexBuffer.hpp>

namespace ciri {
	GLIndexBuffer::GLIndexBuffer()
		: IIndexBuffer(), _evbo(0) {
	}

	GLIndexBuffer::~GLIndexBuffer() {
		destroy();
	}

	bool GLIndexBuffer::set( int* indices, int indexCount, bool dynamic ) {
		// todo: add remapping (updating) support
		if( _evbo != 0 ) {
			return false;
		}

		glGenBuffers(1, &_evbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _evbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexCount, indices, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// todo: check for fail

		return true;
	}

	void GLIndexBuffer::destroy() {
		if( _evbo != 0 ) {
			glDeleteBuffers(1, &_evbo);
			_evbo = 0;
		}
	}

	GLuint GLIndexBuffer::getEvbo() const {
		return _evbo;
	}
} // ciri