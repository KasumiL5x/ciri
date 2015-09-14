#ifndef __ciri_gfx_cull_mode__
#define __ciri_gfx_cull_mode__

namespace ciri {
	/**
		* Triangle winding order cull mode.
		*/
	enum class CullMode {
		None,            /**< Do not cull triangles regardless of vertex ordering. */
		Clockwise,       /**< Cull triangles with clockwise vertex ordering. */
		CounterClockwise /**< Cull triangles with counter-clockwise vertex ordering. */
	};
} // ciri

#endif /* __ciri_gfx_cull_mode__ */