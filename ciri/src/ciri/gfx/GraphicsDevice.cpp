#include <ciri/gfx/GraphicsDevice.hpp>
#include <ciri/wnd/Window.hpp>

namespace ciri {
	GraphicsDevice::GraphicsDevice() {
	}

	GraphicsDevice::~GraphicsDevice() {
	}

	bool GraphicsDevice::create( Window* window ) {
		return _platform.create(window);
	}
} // ciri