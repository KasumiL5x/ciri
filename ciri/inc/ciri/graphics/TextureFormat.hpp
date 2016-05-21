#ifndef __ciri_graphics_TextureFormat__
#define __ciri_graphics_TextureFormat__

namespace ciri {

struct TextureFormat {
	/**
		* Pixel format of texture data.
		*/
	enum Format {
		RGBA32_UINT=0, /**< RGBA; 8-bit unsigned integer per channel; 32 bits total. */
		RGBA32_Float,  /**< RGBA; 32-bit float per channel; 128 bits total. */
		R32_UINT,      /**< R; 32-bit unsigned integer; 32 bits total. */
		R32_FLOAT      /**< R; 32-bit float; 32 bits total. */
	};

	/**
		* Gets the number of bytes per pixel in a given format.
		* [MonoGame](MonoGame.Framework.Graphics.GraphicsExtensions.cs: GetSize).
		* @param format Format to parse.
		* @return Number of bytes per pixel in the given format.
		*/
	static int bytesPerPixel( Format format ) {
		switch( format ) {
			case RGBA32_UINT: {
				return 4; // 32 bits = 4 bytes
			}

			case RGBA32_Float: {
				return 16; // 128 bits = 16 bytes
			}

			case R32_UINT:
			case R32_FLOAT: {
				return 4; // 32 bits = 4 bytes
			}

			default: {
				throw;
			}
		}
	}

	/**
	 * Gets the number of channels for a single pixel.  For instance, RGBA contains four channels.
	 * @param format Format to parse.
	 * @returns Number of channels per pixel in the given format.
	 */
	static int channelsPerPixel( Format format ) {
		switch( format ) {
			case RGBA32_UINT:
			case RGBA32_Float: {
				return 4; // RGBA
			}

			case R32_UINT:
			case R32_FLOAT: {
				return 1; // R
			}
			
			default: {
				throw;
			}
		}
	}

	/**
	 * Gets the number of bytes for a single channel.
	 * @param format Format to parse.
	 * @returns Number of bytes per channel in the given format.
	 */
	static int bytesPerChannel( Format format ) {
		switch( format ) {
			case RGBA32_UINT: {
				return 1;
			}

			case RGBA32_Float: {
				return 4;
			}

			case R32_UINT:
			case R32_FLOAT: {
					return 4;
			}

			default: {
				throw;
			}
		}
	}

	/**
	 * Gets the best alignment for a given format.
	 * Most formats are good multiples of four, but sometimes 1 is required for better alignment.
	 * @param format Format to parse.
	 * @returns Compatible alignment for the given format.
	 */
	static int getAlignment( Format format ) {
		switch( format ) {
			case RGBA32_UINT:
			case RGBA32_Float:
			case R32_UINT:
			case R32_FLOAT: {
				return 4; // all multiples of 4; only use 1 for single bytes etc.
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

}

#endif