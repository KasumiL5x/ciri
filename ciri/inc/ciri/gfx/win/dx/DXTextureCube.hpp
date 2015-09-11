#ifndef __ciri_gfx_dxtexturecube__
#define __ciri_gfx_dxtexturecube__

#include <d3d11.h>
#include <ciri/gfx/ITextureCube.hpp>

namespace ciri {
	class DXGraphicsDevice;

	class DXTextureCube : public ITextureCube {
	public:
		DXTextureCube( DXGraphicsDevice* device );
		virtual ~DXTextureCube();

		virtual ErrorCode set( int width, int height, void* posx, void* negx, void* posy, void* negy, void* posz, void* negz ) override;
		virtual void destroy() override;

		ID3D11Texture2D* getTexture() const;
		ID3D11ShaderResourceView* getShaderResourceView() const;

	private:
		DXGraphicsDevice* _device;
		ID3D11Texture2D* _cubeTexture;
		ID3D11ShaderResourceView* _shaderResourceView;
	};
} // ciri

#endif /* __ciri_gfx_dxtexturecube__ */