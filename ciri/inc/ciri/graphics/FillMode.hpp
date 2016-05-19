#ifndef __ciri_graphics_FillMode__
#define __ciri_graphics_FillMode__

namespace ciri {

/**
	* Fill mode used when rendering.
	*/
enum class FillMode {
	Solid,    /**< Fills triangles formed by vertices. */
	Wireframe /**< Draws lines connecting vertices. */
};

}

#endif