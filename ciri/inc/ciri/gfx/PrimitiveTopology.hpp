#ifndef __ciri_primitivetopology__
#define __ciri_primitivetopology__

namespace ciri {
	struct PrimitiveTopology {
		enum Topology {
			PointList,
			LineList,
			LineStrip,
			TriangleList,
			TriangleStrip,
			MAX
		};
	};
} // ciri

#endif /* __ciri_primitivetopology__ */