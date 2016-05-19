#ifndef __ciri_graphics_VertexUsage__
#define __ciri_graphics_VertexUsage__

namespace ciri { namespace graphics {

/**
	* Defines how a VertexElement is used.
	*/
enum class VertexUsage {
	Position, /**< Treat as position. */
	Color,    /**< Treat as color. */
	Texcoord, /**< Treat as texcoord. */
	Normal,   /**< Treat as normal. */
	Binormal, /**< Treat as binormal. */
	Tangent   /**< Treat as tangent. */
};

}}

#endif