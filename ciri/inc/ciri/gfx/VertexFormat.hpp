#ifndef __ciri_vertexformat__
#define __ciri_vertexformat__

namespace ciri {
	struct VertexFormat {
		/**
		 * Defines the format of a VertexElement.
		 */
		enum Format {
			Float,  /**< Single 32 bit float. */
			Float2, /**< Two 32 bit floats. */
			Float3, /**< Three 32 bit floats. */
			Float4  /**< Four 32 bit floats. */
		};
	};
} // ciri

#endif /* __ciri_vertexformat__ */
