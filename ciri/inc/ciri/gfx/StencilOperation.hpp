#ifndef __ciri_gfx_stencil_operation__
#define __ciri_gfx_stencil_operation__

namespace ciri {
	struct StencilOperation {
		/**
		 * Stencil testing operations.
		 */
		enum Operation {
			Keep,                /**< Keep existing stencil data. */
			Zero,                /**< Set stencil data to zero. */
			Replace,             /**< Set the data to the defined reference value. */
			Increment,           /**< Increment the stencil data by 1. */
			Decrement,           /**< Decrement the stencil data by 1. */
			IncrementSaturation, /**< Increment the stencil data by 1 and clamp the result. */
			DecrementSaturation, /**< Decrement the stencil data by 1 and clamp the result. */
			Invert               /**< Invert the stencil data. */
		};
	};
} // ciri

#endif /* __ciri_gfx_stencil_operation__ */