#include <ciri/gfx/gl/GLRenderTarget2D.hpp>
#include <ciri/gfx/gl/GLTexture2D.hpp>

namespace ciri {
	GLRenderTarget2D::GLRenderTarget2D( GLTexture2D* texture )
		:IRenderTarget2D(), _texture(texture) {
	}

	GLRenderTarget2D::~GLRenderTarget2D() {
		destroy();
	}

	void GLRenderTarget2D::destroy() {
	}

	ITexture2D* GLRenderTarget2D::getTexture2D() const {
		return _texture;
	}
} // ciri