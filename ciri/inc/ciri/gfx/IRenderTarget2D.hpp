#ifndef __ciri_gfx_irendertarget2d__
#define __ciri_gfx_irendertarget2d__

#include <memory>

namespace ciri {
	class ITexture2D;

	class IRenderTarget2D {
	protected:
		IRenderTarget2D() {
		}

	public:
		virtual ~IRenderTarget2D() {
		}

		/**
		 * Uninitializes the render target 2d.
		 */
		virtual void destroy()=0;

		/**
		 * Gets the associated underlying 2d texture of the render target.
		 * @return Pointer to the actual ITexture2D that the render target is representing.
		 */
		virtual std::shared_ptr<ITexture2D> getTexture2D() const=0;
	};
} // ciri

#endif /* __ciri_gfx_irendertarget2d__ */