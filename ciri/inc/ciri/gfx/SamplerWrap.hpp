#ifndef __ciri_samplerwrap__
#define __ciri_samplerwrap__

namespace ciri {
	struct SamplerWrap {
		/**
		 * Behaviors for texture coordinates that are out of texture boundaries.
		 */
		enum Mode {
			Wrap,      /**< Tiles the texture at every UV integer crossing.  Also known as repeat. */
			Mirror,    /**< Flips the texture at every UV integer crossing. */
			Clamp,     /**< Clamps to the last valid color at the closest UV integer crossing. */
			Border,    /**< Uses specified border color for out of bounds coordinates. */
			MirrorOnce /**< Mirrors one time and then clamps. */
		};
	};
} // ciri

#endif /* __ciri_samplerwrap__ */