#include <ciri/gfx/gl/GLTexture2D.hpp>

namespace ciri {
	GLTexture2D::GLTexture2D()
		: ITexture2D() {
	}

	GLTexture2D::~GLTexture2D() {
		destroy();
	}

	void GLTexture2D::destroy() {
	}
} // ciri