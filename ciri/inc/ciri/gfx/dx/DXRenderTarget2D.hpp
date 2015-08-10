#ifndef __ciri_dxrendertarget2d__
#define __ciri_dxrendertarget2d__

#include <d3d11.h>
#include "../IRenderTarget2D.hpp"

namespace ciri {
	class DXGraphicsDevice;
	class DXTexture2D;

	class DXRenderTarget2D : public IRenderTarget2D {
	public:
		DXRenderTarget2D( DXGraphicsDevice* device );
		virtual ~DXRenderTarget2D();

		bool create( DXTexture2D* texture );
		virtual void destroy();

		virtual ITexture2D* getTexture2D() const;

		ID3D11RenderTargetView* getRenderTargetView() const;

	private:
		DXGraphicsDevice* _device;
		DXTexture2D* _texture;
		ID3D11RenderTargetView* _renderTargetView;
	};
} // ciri

#endif /* __ciri_dxrendertarget2d__ */