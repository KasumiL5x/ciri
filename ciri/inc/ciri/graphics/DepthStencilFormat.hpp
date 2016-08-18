#ifndef __ciri_graphics_DepthStencilFormat__
#define __ciri_graphics_DepthStencilFormat__

namespace ciri {

/**
 * Depth-stencil buffer format.
 // https://www.opengl.org/wiki/Image_Format#Depth_stencil_formats
 // https://open.gl/framebuffers
 // https://www.opengl.org/wiki/Framebuffer_Object_Examples
 // http://luugiathuy.com/2011/09/create-frame-buffer-object-opengl/
 */
enum class DepthStencilFormat {
	None,            /**< No buffer. */
	Depth16,         /**< 16-bit normalized integer depth. */
	Depth24,         /**< 24-bit normalized integer depth. */
	Depth32,         /**< 32-bit normalized integer depth. */
	Depth32F,        /**< 32-bit floating point depth. */
	Depth24Stencil8, /**< 24-bit normalized integer depth; 8-bit unsigned integer stencil.  */
	Depth32FStencil8 /**< 32-bit floating point depth; 8-bit unsigned integer stencil. */
};

}

#endif