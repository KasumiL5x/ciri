#ifndef __ciri_isamplerstate__
#define __ciri_isamplerstate__

#include "CompareFunction.hpp"
#include "SamplerWrap.hpp"
#include "SamplerFilter.hpp"

namespace ciri {
	struct SamplerDesc {
		SamplerWrap::Mode wrapU;                  /**< Wrapping method for U texture coordinates that are out of the 0..1 range. */
		SamplerWrap::Mode wrapV;                  /**< Wrapping method for V texture coordinates that are out of the 0..1 range. */
		SamplerWrap::Mode wrapW;                  /**< Wrapping method for W texture coordinates that are out of the 0..1 range. */
		SamplerFilter::Filter filter;             /**< Filtering method used when sampling a texture. */
		int maxAnisotropy;                        /**< Clamping value for anisotropic filtering, if used. */
		float borderColor[4];                     /**< Border color (RGBA) used if wrap modes are set to border. */
		float minLod;                             /**< Sets the minimum LOD, which limits the selection of the highest resolution mipmap (lowest mipmap level). */
		float maxLod;                             /**< Sets the maximum LOD, which limits the selection of the lowest resolution mipmap (highest mipmap level). */
		float lodBias;                            /**< An offset applied to the LOD parameter before texture sampling. */
		CompareFunction::Function comparisonFunc; /**< Function for comparing samples data against existing data. */
		bool useMipmaps;                          /**< Enable or disable the use of mipmaps. */

		SamplerDesc() {
			wrapU = wrapV = wrapW = SamplerWrap::Wrap;
			filter = SamplerFilter::Linear;
			maxAnisotropy = 16;
			borderColor[0] = borderColor[1] = borderColor[2] = borderColor[3] = 0.0f;
			minLod = -1000.0f;
			maxLod = 1000.0f;
			lodBias = 0.0f;
			comparisonFunc = CompareFunction::Always;
			useMipmaps = false;
		}
	};

	class ISamplerState {
	public:
		ISamplerState() {
		}
		virtual ~ISamplerState() {
		}

		/**
		 * Uninitializes the sampler state.
		 */
		virtual void destroy()=0;
	};
} // ciri

#endif /* __ciri_isamplerstate__ */
