#include <ciri/gfx/dx/DXRenderTarget2D.hpp>
#include <ciri/gfx/dx/DXGraphicsDevice.hpp>

namespace ciri {
	DXRenderTarget2D::DXRenderTarget2D( IGraphicsDevice* device )
		: IRenderTarget2D(), _device(device) {
	}

	DXRenderTarget2D::~DXRenderTarget2D() {
		destroy();
	}

	void DXRenderTarget2D::destroy() {
	}

	ITexture2D* DXRenderTarget2D::getTexture2D() const {
		return nullptr;
	}
} // ciri