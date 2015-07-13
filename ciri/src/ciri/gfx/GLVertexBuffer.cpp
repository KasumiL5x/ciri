#include <ciri/gfx/GLVertexBuffer.hpp>

namespace ciri {
	GLVertexBuffer::GLVertexBuffer( GLGraphicsDevice* device )
		: IVertexBuffer(), _device(device) {
	}

	GLVertexBuffer::~GLVertexBuffer() {
	}

	bool GLVertexBuffer::set( void* vertices, int vertexStride, int vertexCount, bool dynamic ) {
		return false;
	}

	void GLVertexBuffer::destroy() {
		// todo
	}

	int GLVertexBuffer::getStride() const {
		return 0;
	}
} // ciri