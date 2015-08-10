#ifndef __ciri_textureflags__
#define __ciri_textureflags__

namespace ciri {
	struct TextureFlags {
		enum Flags {
			RenderTarget = (1 << 0)
		};
	};
} // ciri

#endif /* __ciri_textureflags__ */
