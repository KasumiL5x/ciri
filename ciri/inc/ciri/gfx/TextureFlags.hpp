#ifndef __ciri_textureflags__
#define __ciri_textureflags__

namespace ciri {
	struct TextureFlags {
		enum Flags {
			Mipmaps	     = (1 << 0),
			RenderTarget = (1 << 1)
		};
	};
} // ciri

#endif /* __ciri_textureflags__ */
