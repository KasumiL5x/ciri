#ifndef __ciri_graphics_TextureFormat__
#define __ciri_graphics_TextureFormat__

#include "DepthStencilFormat.hpp"

namespace ciri {

struct TextureFormat {
	/**
		* Pixel format of texture data.
		*/
	enum Format {
		None,
		RGBA32_UINT, /**< RGBA; 8-bit unsigned integer per channel; 32 bits total. */
		RGBA32_Float,  /**< RGBA; 32-bit float per channel; 128 bits total. */
		R32_UINT,      /**< R; 32-bit unsigned integer; 32 bits total. */
		R32_FLOAT,     /**< R; 32-bit float; 32 bits total. */
		//
		// copy of DepthStencilFormat b/c of being a depth enum for render targets but being needed here too.
		// may eventually just fall back to this enum and have one.  see DepthStencilFormat for details.
		Depth16,
		Depth24,
		Depth32,
		Depth32F,
		Depth24Stencil8,
		Depth32FStencil8
	};

	static Format fromDepthStencilFormat( DepthStencilFormat depth ) {
		#define D_TO_F(x)case DepthStencilFormat::x:{return Format::x;}
		switch( depth ) {
			D_TO_F(None)
			D_TO_F(Depth16)
			D_TO_F(Depth24)
			D_TO_F(Depth32)
			D_TO_F(Depth32F)
			D_TO_F(Depth24Stencil8)
			D_TO_F(Depth32FStencil8)
		}
		#undef D_TO_F
	}

	static bool isDepth( Format format ) {
		return (Depth16==format || Depth24==format || Depth32==format || Depth32F==format || Depth24Stencil8==format || Depth32FStencil8==format);
	}

	static bool hasStencil( Format format ) {
		return (Depth24Stencil8==format || Depth32FStencil8==format);
	}

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

			case Depth24:
			case Depth24Stencil8: {
				return 4; // guessing
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

			case Depth16:
			case Depth24:
			case Depth32:
			case Depth32F:
			case Depth24Stencil8:
			case Depth32FStencil8: {
				return 4; // no clue, just guessing	
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

	/**
	 * Gets if the format is floating point data.
	 */
	static bool isFloat( Format format ) {
		return (RGBA32_Float==format || R32_FLOAT==format || Depth32F==format || Depth32FStencil8==format);
	}
};

}

#endif