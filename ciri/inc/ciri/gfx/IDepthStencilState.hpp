#ifndef __ciri_idepthstencilstate__
#define __ciri_idepthstencilstate__

namespace ciri {
	struct StencilOperation {
		enum Operation {
			Keep,
			Zero,
			Replace,
			Increment,
			Decrement,
			IncrementSaturation,
			DecrementSaturation,
			Invert
		};
	};

	struct CompareFunction {
		enum Function {
			Always,
			Never,
			Less,
			LessEqual,
			Equal,
			GreaterEqual,
			Greater,
			NotEqual
		};
	};

	struct DepthStencilDesc {
		bool depthEnable;
		bool depthWriteMask;
		CompareFunction::Function depthFunc;
		//
		bool stencilEnable;
		int stencilReadMask;
		int stencilWriteMask;
		//
		StencilOperation::Operation frontStencilFailOp;
		StencilOperation::Operation frontStencilDepthFailOp;
		StencilOperation::Operation frontStencilPassOp;
		CompareFunction::Function frontStencilFunc;
		//
		StencilOperation::Operation backStencilFailOp;
		StencilOperation::Operation backStencilDepthFailOp;
		StencilOperation::Operation backStencilPassOp;
		CompareFunction::Function backStencilFunc;
		//
		bool twoSidedStencil;
		int stencilRef;

		DepthStencilDesc() {
			depthEnable = true;
			depthWriteMask = true;
			depthFunc = CompareFunction::Less;
			//
			stencilEnable = false;
			stencilReadMask = 1;
			stencilWriteMask = 1;
			//
			frontStencilFailOp = StencilOperation::Keep;
			frontStencilDepthFailOp = StencilOperation::Keep;
			frontStencilPassOp = StencilOperation::Keep;
			frontStencilFunc = CompareFunction::Always;
			//
			backStencilFailOp = StencilOperation::Keep;
			backStencilDepthFailOp = StencilOperation::Keep;
			backStencilPassOp = StencilOperation::Keep;
			backStencilFunc = CompareFunction::Always;
			//
			twoSidedStencil = true;
			stencilRef = 0;
		}
	};

	class IDepthStencilState {
	public:
		IDepthStencilState() {
		}
		virtual ~IDepthStencilState() {
		}

		virtual void destroy()=0;
	};
} // ciri

#endif /* __ciri_idepthstencilstate__ */