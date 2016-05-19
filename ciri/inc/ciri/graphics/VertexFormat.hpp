#ifndef __ciri_graphics_VertexFormat__
#define __ciri_graphics_VertexFormat__

namespace ciri {

/**
	* Defines the format of a VertexElement.
	*/
enum class VertexFormat {
	Float,  /**< Single 32 bit float. */
	Float2, /**< Two 32 bit floats. */
	Float3, /**< Three 32 bit floats. */
	Float4  /**< Four 32 bit floats. */
};

}
#endif