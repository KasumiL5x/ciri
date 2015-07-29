#include <ciri/gfx/gl/GLRenderTarget2D.hpp>

namespace ciri {
	GLRenderTarget2D::GLRenderTarget2D()
		:IRenderTarget2D() {
	}

	GLRenderTarget2D::~GLRenderTarget2D() {
		destroy();
	}

	void GLRenderTarget2D::destroy() {
	}
} // ciri