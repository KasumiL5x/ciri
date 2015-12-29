#ifndef __ciri_gfx_texture_format__
#define __ciri_gfx_texture_format__

namespace ciri {
	struct TextureFormat {
		/**
		 * Pixel format of texture data.
		 */
		enum Format {
			RGBA32_UINT=0, /**< RGBA; 8-bit unsigned integer per channel; 32 bits total. */
			RGBA32_Float   /**< RGBA; 32-bit float per channel; 128 bits total. */
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
} // ciri

#endif /* __ciri_gfx_texture_format__ */
