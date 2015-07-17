#include <ciri/gfx/dx/DXConstantBuffer.hpp>

namespace ciri {
	DXConstantBuffer::DXConstantBuffer()
		: IConstantBuffer() {
	}

	DXConstantBuffer::~DXConstantBuffer() {
		destroy();
	}

	err::ErrorCode DXConstantBuffer::initialize() {
		return err::CIRI_UNKNOWN_ERROR;
	}

	void DXConstantBuffer::destroy() {
	}
} // ciri