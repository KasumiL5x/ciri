#ifndef __ciri_graphics_IRasterizerState__
#define __ciri_graphics_IRasterizerState__

#include "CullMode.hpp"
#include "FillMode.hpp"

namespace ciri {

struct RasterizerDesc {
	CullMode cullMode;   /**< Culls certain a vertex winding order. */
	float depthBias;           /**< Depth value added to a given pixel. */
	FillMode fillMode;   /**< Fill mode to use when rendering. */
	bool msaa;                 /**< Enable or disable multisample antialiasing. */
	bool scissorTestEnable;    /**< Enable or disable scissor-rectangle culling.  All pixels outside an active scissor rectangle are culled. */
	float slopeScaleDepthBias; /**< Scalar on a given pixel's slope. */
	bool depthClipEnable;      /**< Enable or disable clipping based on distance. */

	RasterizerDesc()
		: cullMode(CullMode::CounterClockwise), depthBias(0.0f), fillMode(FillMode::Solid), msaa(false),
			scissorTestEnable(false), slopeScaleDepthBias(0.0f), depthClipEnable(true) {
	}
};

class IRasterizerState {
protected:
	IRasterizerState() {
	}

public:
	virtual ~IRasterizerState() {
	}

	/**
		* Uninitializes the rasterizer state.
		*/
	virtual void destroy()=0;
};

}

#endif