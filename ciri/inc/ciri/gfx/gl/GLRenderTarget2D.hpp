#ifndef __ciri_glrendertarget2d__
#define __ciri_glrendertarget2d__

#include "../IRenderTarget2D.hpp"

namespace ciri {
	class GLRenderTarget2D : public IRenderTarget2D {
	public:
		GLRenderTarget2D();
		virtual ~GLRenderTarget2D();

		virtual void destroy();
	};
} // ciri

#endif /* __ciri_glrendertarget2d__ */