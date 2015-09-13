#include <ciri/gfx/win/gl/GLRenderTarget2D.hpp>
#include <ciri/gfx/win/gl/GLTexture2D.hpp>

namespace ciri {
	GLRenderTarget2D::GLRenderTarget2D( const std::shared_ptr<GLTexture2D>& texture )
		:IRenderTarget2D(), _texture(texture) {
	}

	GLRenderTarget2D::~GLRenderTarget2D() {
		destroy();
	}

	void GLRenderTarget2D::destroy() {
	}

	std::shared_ptr<ITexture2D> GLRenderTarget2D::getTexture2D() const {
		return _texture;
	}
} // ciri