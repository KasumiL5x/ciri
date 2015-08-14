#ifndef __ciri_shaderstage__
#define __ciri_shaderstage__

namespace ciri {
	struct ShaderStage {
		enum Stage {
			Vertex   = (1 << 0),
			Geometry = (1 << 1),
			Pixel    = (1 << 2),
			All      = (1 << 3)
		};
	};
} // ciri

#endif /* __ciri_shaderstage__ */