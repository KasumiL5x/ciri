#ifndef __ciri_graphics_StencilOperation__
#define __ciri_graphics_StencilOperation__

namespace ciri { namespace graphics {

/**
	* Stencil testing operations.
	*/
enum class StencilOperation {
	Keep,                /**< Keep existing stencil data. */
	Zero,                /**< Set stencil data to zero. */
	Replace,             /**< Set the data to the defined reference value. */
	Increment,           /**< Increment the stencil data by 1. */
	Decrement,           /**< Decrement the stencil data by 1. */
	IncrementSaturation, /**< Increment the stencil data by 1 and clamp the result. */
	DecrementSaturation, /**< Decrement the stencil data by 1 and clamp the result. */
	Invert               /**< Invert the stencil data. */
};

}}

#endif