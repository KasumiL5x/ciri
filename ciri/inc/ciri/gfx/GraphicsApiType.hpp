#ifndef __ciri_graphicsapitype__
#define __ciri_graphicsapitype__

namespace ciri {
	struct GraphicsApiType {
		enum Type {
			OpenGL,
			DirectX11
		};
	};
} // ciri

#endif /* __ciri_graphicsapitype__ */
