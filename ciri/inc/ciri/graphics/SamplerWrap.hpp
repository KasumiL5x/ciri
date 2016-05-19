#ifndef __ciri_graphics_SamplerWrap__
#define __ciri_graphics_SamplerWrap__

namespace ciri {

/**
	* Behaviors for texture coordinates that are out of texture boundaries.
	*/
enum class SamplerWrap {
	Wrap,      /**< Tiles the texture at every UV integer crossing.  Also known as repeat. */
	Mirror,    /**< Flips the texture at every UV integer crossing. */
	Clamp,     /**< Clamps to the last valid color at the closest UV integer crossing. */
	Border,    /**< Uses specified border color for out of bounds coordinates. */
	MirrorOnce /**< Mirrors one time and then clamps. */
};

}

#endif