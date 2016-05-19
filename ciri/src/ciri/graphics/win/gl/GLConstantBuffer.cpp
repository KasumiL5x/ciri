#include <ciri/graphics/win/gl/GLConstantBuffer.hpp>

using namespace ciri;

GLConstantBuffer::GLConstantBuffer( GLuint index )
	: IConstantBuffer(), _ubo(0), _index(index) {
}

GLConstantBuffer::~GLConstantBuffer() {
	destroy();
}

ErrorCode GLConstantBuffer::setData( int dataSize, void* data ) {
	// create and set if the buffer doesn't exist (i think glBufferData must be called before glBufferSubData can be)
	if( 0 == _ubo ) {
		glGenBuffers(1, &_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
		glBufferData(GL_UNIFORM_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, _index, _ubo); // _index is set by the graphics device in the constructor
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		return ErrorCode::CIRI_OK;
	}

	glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, dataSize, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return ErrorCode::CIRI_OK;
}

void GLConstantBuffer::destroy() {
	if( _ubo != 0 ) {
		glDeleteBuffers(1, &_ubo);
		_ubo = 0;
	}
	// HACK: Do not reset the index in case the object is reloaded (e.g. reload shaders)
	//_index = 0; // todo: is there any way to unbind it? according to the docs, using glBindBufferBase w/ 0 is not valid
}

GLuint GLConstantBuffer::getUbo() const {
	return _ubo;
}

GLuint GLConstantBuffer::getIndex() const {
	return _index;
}