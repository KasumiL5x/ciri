#include <ciri/gfx/dx/DXTextureCube.hpp>

namespace ciri {
	DXTextureCube::DXTextureCube( DXGraphicsDevice* device )
		: ITextureCube(), _device(device) {
	}

	DXTextureCube::~DXTextureCube() {
	}

	err::ErrorCode DXTextureCube::set( int width, int height, void* right, void* left, void* top, void* bottom, void* back, void* front ) {
		// http://stackoverflow.com/questions/19364012/d3d11-creating-a-cube-map-from-6-images
		// http://www.braynzarsoft.net/index.php?p=D3D11CUBEMAP
		// http://www.hlsl.co.uk/blog/2014/11/19/creating-a-cubemap-in-dx11
		throw;
	}

	void DXTextureCube::destroy() {
		throw;
	}
} // ciri
