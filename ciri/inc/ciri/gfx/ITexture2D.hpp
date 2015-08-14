#ifndef __ciri_itexture__
#define __ciri_itexture__

#include "TextureFlags.hpp"
#include "TextureFormat.hpp"
#include "../ErrorCodes.hpp"

namespace ciri {
	class ITexture2D {
	public:
		ITexture2D( int flags ) {
		}
		virtual ~ITexture2D() {
		}

		virtual void destroy()=0;

		virtual err::ErrorCode setData( int xOffset, int yOffset, int width, int height, void* data, TextureFormat::Format format )=0;

		virtual int getWidth() const=0;
		virtual int getHeight() const=0;

		virtual err::ErrorCode writeToTGA( const char* file )=0;
		virtual err::ErrorCode writeToDDS( const char* file )=0;
	};
} // ciri

#endif /* __ciri_itexture__ */