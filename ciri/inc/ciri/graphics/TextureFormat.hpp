#ifndef __ciri_graphics_TextureFormat__
#define __ciri_graphics_TextureFormat__

namespace ciri { namespace graphics {

struct TextureFormat {
	/**
		* Pixel format of texture data.
		*/
	enum Format {
		RGBA32_UINT=0, /**< RGBA; 8-bit unsigned integer per channel; 32 bits total. */
		RGBA32_Float,  /**< RGBA; 32-bit float per channel; 128 bits total. */
		R32_UINT       /**< R; 32-bit unsigned integer; 32 bits total. */
	};

	/**
		* Gets the number of bytes per pixel in a given format.
		* [MonoGame](MonoGame.Framework.Graphics.GraphicsExtensions.cs: GetSize).
		* @param format Format to parse.
		* @return Number of bytes per pixel in the given format.
		*/
	static int bytesPerPixel( Format format ) {
		switch( format ) {
			case RGBA32_UINT:
			case RGBA32_Float: {
				return 4;
			}

			case R32_UINT: {
				return 1;
			}

			default: {
				throw;
			}
		}
	}

	/**
		* Checks if a given format has alpha or not.
		* @param format Format to check.
		* @return True if the format has alpha.
		*/
	static bool hasAlpha( Format format ) {
		return (RGBA32_UINT == format) ||
						(RGBA32_Float == format);
	}
};

}}

#endif