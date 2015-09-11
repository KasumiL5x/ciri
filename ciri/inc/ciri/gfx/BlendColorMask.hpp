#ifndef __ciri_gfx_blend_color_mask__
#define __ciri_gfx_blend_color_mask__

namespace ciri {
	/**
	 * Bitflags for color channel write mask for blending operations.
	 */
	struct BlendColorMask {
		enum Mask {
			None  = 0,        /**< No channels. */
			Red   = (1 << 0), /**< Red channel. */
			Green = (1 << 1), /**< Green channel. */
			Blue  = (1 << 2), /**< Blue channel. */
			Alpha = (1 << 3), /**< Alpha channel. */
			All   = (Red | Green | Blue | Alpha) /**< All channels. */
		};
	};
} // ciri

#endif /* __ciri_gfx_blend_color_mask__ */