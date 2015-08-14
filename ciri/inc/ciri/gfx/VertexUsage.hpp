#ifndef __ciri_vertexusage__
#define __ciri_vertexusage__

namespace ciri {
	struct VertexUsage {
		enum Usage {
			Position,
			Color,
			Texcoord,
			Normal,
			Binormal,
			Tangent,
			MAX
		};
	};
} // ciri

#endif /* __ciri_vertexusage__ */