#ifndef __ciri_primitivetopology__
#define __ciri_primitivetopology__

namespace ciri {
	class PrimitiveTopology {
	public:
		enum Type {
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