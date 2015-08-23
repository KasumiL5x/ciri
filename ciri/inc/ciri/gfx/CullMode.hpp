#ifndef __ciri_cullmode__
#define __ciri_cullmode__

namespace ciri {
	struct CullMode {
		/**
		 * Triangle winding order cull mode.
		 */
		enum Mode {
			None,            /**< Do not cull triangles regardless of vertex ordering. */
			Clockwise,       /**< Cull triangles with clockwise vertex ordering. */
			CounterClockwise /**< Cull triangles with counter-clockwise vertex ordering. */
		};
	};
} // ciri

#endif /* __ciri_cullmode__ */