#ifndef __ciri_graphics_ShaderStage__
#define __ciri_graphics_ShaderStage__

namespace ciri { namespace graphics {

struct ShaderStage {
	/**
		* Bitflags for shader stages.
		*/
	enum Stage {
		Vertex   = (1 << 0), /**< Vertex shader stage bit. */
		Geometry = (1 << 1), /**< Geometry shader stage bit. */
		Pixel    = (1 << 2), /**< Pixel shader stage bit. */
		All      = (1 << 3)  /**< All (vertex, geometry, pixel) stages bit. */
	};
};

}}

#endif