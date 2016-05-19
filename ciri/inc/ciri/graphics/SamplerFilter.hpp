#ifndef __ciri_graphics_SamplerFilter__
#define __ciri_graphics_SamplerFilter__

namespace ciri { namespace graphics {

/**
	* Filtering options for texture sampling.
	* 
	* <b>Notes on terminology:</b>
	* \li '<b>min</b>' refers to the filtering method used for minification.
	* 
	* \li '<b>mag</b>' refers to the filtering method used for magnification.
	* 
	* \li '<b>mip</b>' refers to the filtering method used for mip-level sampling.
	*/
enum class SamplerFilter {
	Point,                      /**< min=point; mag=point; mip=point. */
	Linear,                     /**< min=linear; mag=linear; mip=linear. */
	Anisotropic,                /**< min=aniso; mag=aniso; mip=aniso. */
	LinearMipPoint,             /**< min=linear; mag=linear; mip=point. */
	PointMipLinear,             /**< min=point; mag=point; mip=linear. */
	MinLinearMagPointMipLinear, /**< min=linear; mag=point; mip=linear. */
	MinLinearMagPointMipPoint,  /**< min=linear; mag=point; mip=point. */
	MinPointMagLinearMipLinear, /**< min=point; mag=linear; mip=linear. */
	MinPointMagLinearMipPoint   /**< min=point; mag=linear; mip=point. */
};

}}

#endif