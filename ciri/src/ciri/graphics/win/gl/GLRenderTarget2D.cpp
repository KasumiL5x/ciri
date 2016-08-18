#include <ciri/graphics/win/gl/GLRenderTarget2D.hpp>
#include <ciri/graphics/win/gl/GLTexture2D.hpp>

using namespace ciri;

GLRenderTarget2D::GLRenderTarget2D()
	:IRenderTarget2D(), _texture(nullptr), _depth(nullptr) {
}

GLRenderTarget2D::~GLRenderTarget2D() {
	destroy();
}

bool GLRenderTarget2D::create( const std::shared_ptr<GLTexture2D>& texture, const std::shared_ptr<GLTexture2D>& depth ) {
	_texture = texture;
	_depth = depth;
	return true;
}

void GLRenderTarget2D::destroy() {
}

std::shared_ptr<ITexture2D> GLRenderTarget2D::getTexture() const {
	return _texture;
}

std::shared_ptr<ITexture2D> GLRenderTarget2D::getDepth() const {
	return _depth;
}