#ifndef __ciri_isamplerstate__
#define __ciri_isamplerstate__

namespace ciri {

	struct SamplerWrap {
		enum Mode {
			Wrap,
			Mirror,
			Clamp,
			Border,
			MirrorOnce
		};
	};

	struct SamplerFilter {
		enum Filter {
			Point,
			Linear,
			Anisotropic,
			LinearMipPoint,
			PointMipLinear,
			MinLinearMagPointMipLinear,
			MinLinearMagPointMipPoint,
			MinPointMagLinearMipLinear,
			MinPointMagLinearMipPoint
		};
	};

	struct SamplerComparison {
		enum Mode {
			Never,
			Always,
			Less,
			Equal,
			LessEqual,
			Greater,
			GreaterEqual,
			NotEqual
		};
	};

	struct SamplerDesc {
		SamplerWrap::Mode wrapU;
		SamplerWrap::Mode wrapV;
		SamplerWrap::Mode wrapW;
		SamplerFilter::Filter filter;
		int maxAnisotropy;
		float borderColor[4];
		float minLod;
		float maxLod;
		float lodBias;
		SamplerComparison::Mode comparisonFunc;
		bool useMipmaps;

		SamplerDesc() {
			wrapU = wrapV = wrapW = SamplerWrap::Wrap;
			filter = SamplerFilter::Linear;
			maxAnisotropy = 16;
			borderColor[0] = borderColor[1] = borderColor[2] = borderColor[3] = 0.0f;
			minLod = -1000.0f;
			maxLod = 1000.0f;
			lodBias = 0.0f;
			comparisonFunc = SamplerComparison::Always;
			useMipmaps = false;
		}
	};

	class ISamplerState {
	public:
		ISamplerState() {
		}
		virtual ~ISamplerState() {
		}

		virtual void destroy()=0;
	};
} // ciri

#endif /* __ciri_isamplerstate__ */