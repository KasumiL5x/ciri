#ifndef __ciri_dxtexture2d__
#define __ciri_dxtexture2d__

#include "../ITexture2D.hpp"

namespace ciri {
	class DXTexture2D : public ITexture2D {
	public:
		DXTexture2D();
		virtual ~DXTexture2D();

		virtual void destroy();
		virtual bool setData( int xOffset, int yOffset, int width, int height, void* data, TextureFormat::Type format );
	};
}

#endif /* __ciri_dxtexture2d__ */