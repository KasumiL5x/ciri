#ifndef __ciri_irendertarget2d__
#define __ciri_irendertarget2d__

namespace ciri {
	class IRenderTarget2D {
	public:
		IRenderTarget2D() {
		}

		virtual ~IRenderTarget2D() {
		}

		virtual void destroy()=0;
	};
} // ciri

#endif /* __ciri_irendertarget2d__ */