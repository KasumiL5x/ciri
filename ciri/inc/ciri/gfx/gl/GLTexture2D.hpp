#ifndef __ciri_gltexture2d__
#define __ciri_gltexture2d__

#include "../ITexture2D.hpp"

namespace ciri {
	class GLTexture2D : public ITexture2D {
	public:
		GLTexture2D();
		virtual ~GLTexture2D();

		virtual void destroy();
	};
} // ciri

#endif /* __ciri_gltexture2d__ */