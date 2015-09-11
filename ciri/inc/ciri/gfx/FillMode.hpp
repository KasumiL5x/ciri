#ifndef __ciri_gfx_fill_mode__
#define __ciri_gfx_fill_mode__

namespace ciri {
	struct FillMode {
		/**
		 * Fill mode used when rendering.
		 */
		enum Mode {
			Solid,    /**< Fills triangles formed by vertices. */
			Wireframe /**< Draws lines connecting vertices. */
		};
	};
} // ciri

#endif /* __ciri_gfx_fill_mode__ */