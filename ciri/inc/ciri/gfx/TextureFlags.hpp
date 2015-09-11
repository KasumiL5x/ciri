#ifndef __ciri_gfx_texture_flags__
#define __ciri_gfx_texture_flags__

namespace ciri {
	struct TextureFlags {
		/**
		 * Texture flags.
		 */
		enum Flags {
			Mipmaps	     = (1 << 0), /**< If present, mipmaps will be generated. */
			RenderTarget = (1 << 1)  /**< If present, the texture is a render target. */
		};
	};
} // ciri

#endif /* __ciri_gfx_texture_flags__ */