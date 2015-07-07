#include "DXGraphicsDevice.hpp"

namespace ciri {
	DXGraphicsDevice::DXGraphicsDevice()
		: IGraphicsDevice() {
	}

	DXGraphicsDevice::~DXGraphicsDevice() {
	}

	bool DXGraphicsDevice::create( Window* window ) {
		return _platform.create(window);
	}
}