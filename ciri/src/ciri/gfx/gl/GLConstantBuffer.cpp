#include <ciri/gfx/gl/GLConstantBuffer.hpp>

namespace ciri {
	GLConstantBuffer::GLConstantBuffer( GLuint index )
		: IConstantBuffer(), _ubo(0), _index(index) {
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
			glBindBufferBase(GL_UNIFORM_BUFFER, _index, _ubo); // _index is set by the graphics device in the constructor
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			return err::ErrorCode::CIRI_OK;
		}

		glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, dataSize, data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		return err::ErrorCode::CIRI_OK;
	}

	void GLConstantBuffer::destroy() {
		if( _ubo != 0 ) {
			glDeleteBuffers(1, &_ubo);
			_ubo = 0;
		}
		_index = 0; // todo: is there ay way to unbind it? according to the docs, using glBindBufferBase w/ 0 is not valid
	}

	GLuint GLConstantBuffer::getUbo() const {
		return _ubo;
	}

	GLuint GLConstantBuffer::getIndex() const {
		return _index;
	}
} // ciri