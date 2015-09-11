#ifndef __ciri_gfx_clear_flags__
#define __ciri_gfx_clear_flags__

namespace ciri {
	struct ClearFlags {
		/**
		 * Bitflags for clearing.
		 */
		enum Flags {
			Color		=	(1 << 0), /**< Color channel bit. */
			Depth		=	(1 << 1), /**< Depth channel bit. */
			Stencil	=	(1 << 2)  /**< Stencil channel bit. */
		};
	};
} // ciri

#endif /* __ciri_gfx_clear_flags__ */