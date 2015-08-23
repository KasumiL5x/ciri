#ifndef __ciri_fillmode__
#define __ciri_fillmode__

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

#endif /* __ciri_fillmode__ */