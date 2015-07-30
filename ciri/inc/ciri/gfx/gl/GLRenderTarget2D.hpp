#ifndef __ciri_glrendertarget2d__
#define __ciri_glrendertarget2d__

#include "../IRenderTarget2D.hpp"

namespace ciri {
	class GLTexture2D;

	class GLRenderTarget2D : public IRenderTarget2D {
	public:
		GLRenderTarget2D( GLTexture2D* texture );
		virtual ~GLRenderTarget2D();

		virtual void destroy();

		virtual ITexture2D* getTexture2D() const;

	private:
		GLTexture2D* _texture;
	};
} // ciri

#endif /* __ciri_glrendertarget2d__ */