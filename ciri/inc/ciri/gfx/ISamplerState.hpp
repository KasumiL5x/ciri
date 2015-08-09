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
			Point, // min=point; mag=point; mip=point
			PointLinear, // min=point; mag=linear; mip=point
			LinearPoint, // min=linear; mag=point; mip=point
			Bilinear, // min=linear; mag=linear; mip=point
			Trilinear, // min=linear; mag=linear, mip=linear
			Anisotropic // dank filterz
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
		SamplerFilter::Mode filter;
		int maxAnisotropy;
		float borderColor[4];
		float minLod;
		float maxLod;
		float lodBias;
		SamplerComparison::Mode comparisonFunc;

		SamplerDesc() {
			wrapU = wrapV = wrapW = SamplerWrap::Wrap;
			filter = SamplerFilter::Bilinear;
			maxAnisotropy = 16;
			borderColor[0] = borderColor[1] = borderColor[2] = borderColor[3] = 0.0f;
			minLod = -1000.0f;
			maxLod = 1000.0f;
			lodBias = 0.0f;
			comparisonFunc = SamplerComparison::Always;
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