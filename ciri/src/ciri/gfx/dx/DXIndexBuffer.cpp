#include <ciri/gfx/dx/DXIndexBuffer.hpp>

namespace ciri {
	DXIndexBuffer::DXIndexBuffer()
		: IIndexBuffer() {
	}

	DXIndexBuffer::~DXIndexBuffer() {
		destroy();
	}

	void DXIndexBuffer::destroy() {
	}
} // ciri