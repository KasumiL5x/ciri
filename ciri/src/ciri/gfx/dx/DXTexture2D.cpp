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
} // ciri