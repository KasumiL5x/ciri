#include <ciri/graphics/win/gl/GLRenderTarget2D.hpp>
#include <ciri/graphics/win/gl/GLTexture2D.hpp>

using namespace ciri::graphics;

GLRenderTarget2D::GLRenderTarget2D()
	:IRenderTarget2D(), _texture(nullptr) {
}

GLRenderTarget2D::~GLRenderTarget2D() {
	destroy();
}

bool GLRenderTarget2D::create( const std::shared_ptr<GLTexture2D>& texture ) {
	_texture = texture;
	return true;
}

void GLRenderTarget2D::destroy() {
}

std::shared_ptr<ITexture2D> GLRenderTarget2D::getTexture2D() const {
	return _texture;
}