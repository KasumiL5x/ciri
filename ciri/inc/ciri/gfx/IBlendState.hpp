#ifndef __ciri_iblendstate__
#define __ciri_iblendstate__

#include "BlendFunction.hpp"
#include "BlendMode.hpp"
#include "BlendColorMask.hpp"

namespace ciri {
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
			blendFactor[0] = blendFactor[1] = blendFactor[2] = blendFactor[3] = 0.0f;
			colorFunc = BlendFunction::Add;
			alphaFunc = BlendFunction::Add;
			colorMask = static_cast<int>(BlendColorMask::All);
		}
	};

	class IBlendState {
	public:
		IBlendState() {
		}
		virtual ~IBlendState() {
		}

		/**
		 * Uninitializes the blend state.
		 */
		virtual void destroy()=0;
	};
} // ciri

#endif /* __ciri_iblendstate__ */
