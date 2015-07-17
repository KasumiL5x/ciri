#include <ciri/gfx/dx/DXConstantBuffer.hpp>

namespace ciri {
	DXConstantBuffer::DXConstantBuffer()
		: IConstantBuffer() {
	}

	DXConstantBuffer::~DXConstantBuffer() {
		destroy();
	}

	err::ErrorCode DXConstantBuffer::setData( int dataSize, void* data ) {
		return err::CIRI_UNKNOWN_ERROR;
	}

	void DXConstantBuffer::destroy() {
	}
} // ciri