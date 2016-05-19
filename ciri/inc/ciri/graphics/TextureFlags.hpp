#ifndef __ciri_graphics_TextureFlags__
#define __ciri_graphics_TextureFlags__

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

}

#endif