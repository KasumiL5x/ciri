#ifndef __ciri_blendfunction__
#define __ciri_blendfunction__

namespace ciri {
	/**
	 * Blend functions.
	 */
	enum class BlendFunction {
		Add,             /**< Adds destination to source: (srcColor * srcBlend) + (dstColor * dstBlend) */
		Subtract,        /**< Subtracts destination from source: (srcColor * srcBlend) - (dstColor * dstBlend) */
		ReverseSubtract, /**< Subtracts source from destination: (dstColor * dstBlend) - (srcColor * srcBlend) */
		Min,             /**< Uses minimum of source and destination: min((srcColor * srcBlend), (dstColor * dstBlend)) */
		Max              /**< Uses maximum of source and destination: max((srcColor * srcBlend), (dstColor * dstBlend)) */
	};
} // ciri

#endif /* __ciri_blendfunction__ */
