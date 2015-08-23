#ifndef __ciri_shaderstage__
#define __ciri_shaderstage__

namespace ciri {
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
} // ciri

#endif /* __ciri_shaderstage__ */
