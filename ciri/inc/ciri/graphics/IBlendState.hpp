#ifndef __ciri_graphics_IBlendState__
#define __ciri_graphics_IBlendState__

#include "BlendFunction.hpp"
#include "BlendMode.hpp"
#include "BlendColorMask.hpp"

namespace ciri { namespace graphics {

struct BlendDesc {
	BlendMode srcColorBlend;
	BlendMode dstColorBlend;
	BlendMode srcAlphaBlend;
	BlendMode dstAlphaBlend;
	float blendFactor[4];
	BlendFunction colorFunc;
	BlendFunction alphaFunc;
	int colorMask; /**< Bitflag of BlendColorMask. */

	bool blendingEnabled() const {
		return !((BlendMode::One  == srcColorBlend) &&
							(BlendMode::Zero == dstColorBlend) &&
							(BlendMode::One  == srcAlphaBlend) &&
							(BlendMode::Zero == dstAlphaBlend));
	}

	BlendDesc() {
		srcColorBlend = BlendMode::One;
		dstColorBlend = BlendMode::Zero;
		srcAlphaBlend = BlendMode::One;
		dstAlphaBlend = BlendMode::Zero;
		blendFactor[0] = blendFactor[1] = blendFactor[2] = blendFactor[3] = 1.0f; // todo: dx defaults to 1, gl defaults to 0, wat do?
		colorFunc = BlendFunction::Add;
		alphaFunc = BlendFunction::Add;
		colorMask = static_cast<int>(BlendColorMask::All);
	}
};

class IBlendState {
protected:
	IBlendState() {
	}

public:
	virtual ~IBlendState() {
	}

	/**
		* Uninitializes the blend state.
		*/
	virtual void destroy()=0;
};

}}

#endif