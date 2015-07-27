#ifndef __ciri_isamplerstate__
#define __ciri_isamplerstate__

namespace ciri {

	class SamplerWrap {
	public:
		enum Mode {
			Wrap,
			Mirror,
			Clamp,
			Border,
			MirrorOnce
		};
	};

	class SamplerFilter {
	public:
		enum Mode {
			Nearest,
			Linear,
			Bilinear,
			Trilinear,
			Anisotropic
		};
	};

	class SamplerComparison {
	public:
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
		SamplerFilter::Mode filterMin;
		SamplerFilter::Mode filterMax;
		float borderColor[4];
		float minLod;
		float maxLod;
		float lodBias;
		SamplerComparison::Mode comparisonFunc;
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