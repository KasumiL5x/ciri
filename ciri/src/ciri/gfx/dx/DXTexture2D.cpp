#include <ciri/gfx/dx/DXTexture2D.hpp>

namespace ciri {
	DXTexture2D::DXTexture2D()
		: ITexture2D() {
	}

	DXTexture2D::~DXTexture2D() {
		destroy();
	}

	void DXTexture2D::destroy() {
	}

	bool DXTexture2D::setData( int xOffset, int yOffset, int width, int height, void* data, TextureFormat::Type format ) {
		return false;
	}
} // ciri