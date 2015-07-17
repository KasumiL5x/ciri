#include <ciri/gfx/gl/GLConstantBuffer.hpp>

namespace ciri {
	GLConstantBuffer::GLConstantBuffer()
		: IConstantBuffer() {
	}

	GLConstantBuffer::~GLConstantBuffer() {
		destroy();
	}

	err::ErrorCode GLConstantBuffer::initialize() {
		return err::CIRI_UNKNOWN_ERROR;
	}

	void GLConstantBuffer::destroy() {

	}
} // ciri