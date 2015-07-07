#include "GLGraphicsDevice.hpp"

namespace ciri {
	GLGraphicsDevice::GLGraphicsDevice()
		: IGraphicsDevice() {
	}

	GLGraphicsDevice::~GLGraphicsDevice() {
	}

	bool GLGraphicsDevice::create( Window* window ) {
		return _platform.create(window);
	}

	void GLGraphicsDevice::destroy() {
		_platform.destroy();
	}

	void GLGraphicsDevice::present() {
		_platform.present();
	}
} // ciri