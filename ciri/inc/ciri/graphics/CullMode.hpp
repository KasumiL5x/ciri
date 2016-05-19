#ifndef __ciri_graphics_CullMode__
#define __ciri_graphics_CullMode__

namespace ciri { namespace graphics {

/**
	* Triangle winding order cull mode.
	*/
enum class CullMode {
	None,            /**< Do not cull triangles regardless of vertex ordering. */
	Clockwise,       /**< Cull triangles with clockwise vertex ordering. */
	CounterClockwise /**< Cull triangles with counter-clockwise vertex ordering. */
};

}}

#endif