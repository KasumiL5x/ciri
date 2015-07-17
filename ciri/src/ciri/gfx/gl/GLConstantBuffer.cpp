#include <ciri/gfx/gl/GLConstantBuffer.hpp>

namespace ciri {
	GLConstantBuffer::GLConstantBuffer()
		: IConstantBuffer(), _ubo(0) {
	}

	GLConstantBuffer::~GLConstantBuffer() {
		destroy();
	}

	err::ErrorCode GLConstantBuffer::setData( int dataSize, void* data ) {
		// create and set if the buffer doesn't exist (i think glBufferData must be called before glBufferSubData can be)
		if( 0 == _ubo ) {
			glGenBuffers(1, &_ubo);
			glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
			glBufferData(GL_UNIFORM_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			return err::CIRI_OK;
		}

		glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, dataSize, data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		return err::CIRI_OK;
	}

	void GLConstantBuffer::destroy() {
		if( _ubo != 0 ) {
			glDeleteBuffers(1, &_ubo);
			_ubo = 0;
		}
	}

	GLuint GLConstantBuffer::getUbo() const {
		return _ubo;
	}
} // ciri