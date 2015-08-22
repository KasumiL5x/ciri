#ifndef __ciri_clearflags__
#define __ciri_clearflags__

namespace ciri {
	/**
	 * Bitflags for clearing.
	 */
	struct ClearFlags {
		/**
		 * @see ClearFlags
		 */
		enum Flags {
			Color		=	(1 << 0), /**< Color channel bit. */
			Depth		=	(1 << 1), /**< Depth channel bit. */
			Stencil	=	(1 << 2)  /**< Stencil channel bit. */
		};
	};
} // ciri

#endif /* __ciri_clearflags__ */
