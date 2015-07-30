#ifndef __ciri_itexture__
#define __ciri_itexture__

#include "TextureFormat.hpp"

namespace ciri {
	class ITexture2D {
	public:
		ITexture2D() {
		}
		virtual ~ITexture2D() {
		}

		virtual void destroy()=0;

		virtual bool setData( int xOffset, int yOffset, int width, int height, void* data, TextureFormat::Type format )=0;

		virtual int getWidth() const=0;
		virtual int getHeight() const=0;
	};
} // ciri

#endif /* __ciri_itexture__ */