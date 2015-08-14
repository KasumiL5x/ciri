#ifndef __ciri_vertexformat__
#define __ciri_vertexformat__

namespace ciri {
	struct VertexFormat {
		enum Format {
			Float,  // single 32-bit float
			Float2, // two 32-bit floats
			Float3, // three 32-bit floats
			Float4, // four 32-bit floats
			MAX
		};
	};
} // ciri

#endif /* __ciri_vertexformat__ */