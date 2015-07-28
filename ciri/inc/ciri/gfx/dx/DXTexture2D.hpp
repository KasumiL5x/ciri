#ifndef __ciri_dxtexture2d__
#define __ciri_dxtexture2d__

#include <d3d11_1.h>
#include "../ITexture2D.hpp"

namespace ciri {
	class DXGraphicsDevice;

	class DXTexture2D : public ITexture2D {
	public:
		DXTexture2D( DXGraphicsDevice* device );
		virtual ~DXTexture2D();

		virtual void destroy();
		virtual bool setData( int xOffset, int yOffset, int width, int height, void* data, TextureFormat::Type format );

		ID3D11ShaderResourceView* getShaderResourceView() const;

	private:
		DXGI_FORMAT ciriToDxFormat( TextureFormat::Type format ) const;

	private:
		DXGraphicsDevice* _device;
		ID3D11Texture2D* _texture2D;
		ID3D11ShaderResourceView* _shaderResourceView;
	};
}

#endif /* __ciri_dxtexture2d__ */