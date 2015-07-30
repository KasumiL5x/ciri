#ifndef __ciri_dxrendertarget2d__
#define __ciri_dxrendertarget2d__

#include "../IRenderTarget2D.hpp"

namespace ciri {
	class IGraphicsDevice;

	class DXRenderTarget2D : public IRenderTarget2D {
	public:
		DXRenderTarget2D( IGraphicsDevice* device );
		virtual ~DXRenderTarget2D();

		virtual void destroy();

		virtual ITexture2D* getTexture2D() const;

	private:
		IGraphicsDevice* _device;
	};
} // ciri

#endif /* __ciri_dxrendertarget2d__ */