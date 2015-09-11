#ifndef __ciri_gfx_graphics_api_type__
#define __ciri_gfx_graphics_api_type__

namespace ciri {
	struct GraphicsApiType {
		enum Type {
			OpenGL,
			DirectX11
		};
	};
} // ciri

#endif /* __ciri_gfx_graphics_api_type__ */