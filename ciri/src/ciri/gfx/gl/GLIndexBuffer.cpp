#include <ciri/gfx/gl/GLIndexBuffer.hpp>

namespace ciri {
	GLIndexBuffer::GLIndexBuffer()
		: IIndexBuffer() {
	}

	GLIndexBuffer::~GLIndexBuffer() {
		destroy();
	}

	void GLIndexBuffer::destroy() {
	}
} // ciri