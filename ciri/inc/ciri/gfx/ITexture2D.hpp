#ifndef __ciri_itexture__
#define __ciri_itexture__

namespace ciri {
	class ITexture2D {
	public:
		ITexture2D() {
		}
		virtual ~ITexture2D() {
		}

		virtual void destroy()=0;
	};
} // ciri

#endif /* __ciri_itexture__ */