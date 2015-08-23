#ifndef __ciri_primitivetopology__
#define __ciri_primitivetopology__

namespace ciri {
	struct PrimitiveTopology {
		/**
		 * How vertex data is interpreted when drawing.
		 */
		enum Topology {
			PointList,    /**< Each vertex is a single point.  Vertex <b>n</b> is point <b>n</b>. */
			LineList,     /**< Each pair of vertices are a line.  Vertices <b>2n-1</b> and <b>2n</b> define line <b>n</b>.  <b>n/2</b> lines are drawn. */
			LineStrip,    /**< Draws connected lines from the first vertex to the last.  Vertices <b>n</b> and <b>n+1</b> define line <b>n</b>.  <b>n-1</b> lines are drawn. */
			TriangleList, /**< Three vertices form an independent triangle.  Vertices <b>3n-2</b>, <b>3n-1</b>, and <b>3n</b> define triangle <b>n</b>.  <b>n/3</b> triangles are drawn. */
			TriangleStrip /**< Draws connected triangles.  One triangle is defined for each vertex after the first two.  For odd <b>n</b>, vertices <b>n</b>, <b>n+1</b>, and <b>n+2</b> define triangle <b>n</b>.  For even <b>n</b>, vertices <b>n+1</b>, <b>n</b>, and <b>n+2</b> define triangle <b>n</b>.  <b>n-2</b> triangles are drawn. */
		};
	};
} // ciri

#endif /* __ciri_primitivetopology__ */
