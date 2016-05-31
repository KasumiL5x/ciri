#ifndef __ciri_graphics_DepthFormat__
#define __ciri_graphics_DepthFormat__

namespace ciri {

/**
 * Depth-stencil buffer format.
 */
enum class DepthFormat {
	None,           /**< No buffer. */
	Depth16,        /**< 16-bit depth buffer. */
	Depth24Stencil8 /**< 24-bit depth buffer with 8-bit stencil buffer (32 bits in total). */
};

}

#endif