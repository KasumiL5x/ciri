#ifndef __ciri_graphics_IRenderTarget2D__
#define __ciri_graphics_IRenderTarget2D__

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
		* Uninitializes the render target 2D.
		*/
	virtual void destroy()=0;

	/**
		* Gets the associated underlying 2D texture of the render target.
		* @return Pointer to the actual ITexture2D that the render target is representing.
		*/
	virtual std::shared_ptr<ITexture2D> getTexture() const=0;

	/**
	 * Gets the depth texture of the render target.  This can be null.
	 * @return Pointer to the depth ITexture2D, if it exists.
	 */
	virtual std::shared_ptr<ITexture2D> getDepth() const=0;
};

}

#endif