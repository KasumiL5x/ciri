#ifndef __ciri_gfx_texture_format__
#define __ciri_gfx_texture_format__

namespace ciri {
	struct TextureFormat {
		/**
		 * Pixel format of texture data.
		 */
		enum Format {
			Color = 0,	 /**< Unsigned 32bit ARGB; 8 bits per channel. */
			RGB32_Float, /**< RGB 32 bit float. */
			RGBA32_Float /**< RGBA 32 bit float. */
		};

		/**
		 * Gets the number of bytes per pixel in a given format.
		 * [MonoGame](MonoGame.Framework.Graphics.GraphicsExtensions.cs: GetSize).
		 * @param format Format to parse.
		 * @return Number of bytes per pixel in the given format.
		 */
		static int bytesPerPixel( Format format ) {
			switch( format ) {
				case Color: {
						return 4;
				}

				case RGB32_Float: // 3???
				case RGBA32_Float: {
						return 4;
				}

				default: {
					return 0;
				}
			}
		}

		/**
		 * Checks if a given format has alpha or not.
		 * @param format Format to check.
		 * @return True if the format has alpha.
		 */
		static bool hasAlpha( Format format ) {
			return (Color == format) ||
						 (RGBA32_Float == format);
		}
	};
} // ciri

#endif /* __ciri_gfx_texture_format__ */