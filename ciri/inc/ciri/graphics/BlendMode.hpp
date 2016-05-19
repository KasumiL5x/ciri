#ifndef __ciri_graphics_BlendMode__
#define __ciri_graphics_BlendMode__

namespace ciri {

/**
	* Blending modes.
	*/
enum class BlendMode {
	One,                     /**< Color is multiplied by: {1, 1, 1, 1}. */
	Zero,                    /**< Color is multiplied by: {0, 0, 0, 0}. */
	SourceColor,             /**< Color is multiplied by source color: {srcR, srcG, srcB, srcA}. */
	InverseSourceColor,      /**< Color is multiplied by inverse source color: {1-srcR, 1-srcG, 1-srcB, 1-srcA}. */
	SourceAlpha,             /**< Color is multiplied by source alpha: {srcA, srcA, srcA, srcA}. */
	InverseSourceAlpha,      /**< Color is multiplied by inverse source alpha: {1-srcA, 1-srcA, 1-srcA, 1-srcA}. */
	DestinationColor,        /**< Color is multiplied by destination color: {dstR, dstG, dstB, dstA}. */
	InverseDestinationColor, /**< Color is multiplied by inverse destination color: {1-dstR, 1-dstG, 1-dstB, 1-dstA}. */
	DestinationAlpha,        /**< Color is multiplied by destination alpha: {dstA, dstA, dstA, dstA}. */
	InverseDestinationAlpha, /**< Color is multiplied by inverse destination alpha: {1-dstA, 1-dstA, 1-dstA, 1-dstA}. */
	BlendFactor,             /**< Color is multiplied by blend factor color. */
	InverseBlendFactor,      /**< Color is multiplied by inverse blend factor color. */
	SourceAlphaSaturation    /**< Color is multiplied by max of alpha of source color or its inverse: {c, c, c, 1} where c=min(srcA, 1-srcA). */
};

}

#endif