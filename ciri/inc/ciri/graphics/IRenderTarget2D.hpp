#ifndef __ciri_graphics_IRenderTarget2D__
#define __ciri_graphics_IRenderTarget2D__

#include <memory>

namespace ciri { namespace graphics {

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

}}

#endif