#ifndef __ciri_irasterizerstate__
#define __ciri_irasterizerstate__

namespace ciri {
	struct  FillMode {
		enum Mode {
			Solid,
			Wireframe
		};
	};

	struct CullMode {
		enum Mode {
			None,
			Clockwise,
			CounterClockwise
		};
	};

	struct RasterizerDesc {
		CullMode::Mode cullMode;
		float depthBias;
		FillMode::Mode fillMode;
		bool msaa;
		bool scissorTestEnable;
		float slopeScaleDepthBias;
		bool depthClipEnable;

		RasterizerDesc()
			: cullMode(CullMode::CounterClockwise), depthBias(0.0f), fillMode(FillMode::Solid), msaa(true),
				scissorTestEnable(false), slopeScaleDepthBias(0.0f), depthClipEnable(true) {
		}
	};

	class IRasterizerState {
	public:
		IRasterizerState() {
		}
		virtual ~IRasterizerState() {
		}

		virtual void destroy()=0;
	};
} // ciri

#endif /* __ciri_irasterizerstate__ */