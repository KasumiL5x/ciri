#ifndef __ciri_idepthstencilstate__
#define __ciri_idepthstencilstate__

#include "CompareFunction.hpp"

namespace ciri {
	struct StencilOperation {
		/**
		 * Stencil testing operations.
		 */
		enum Operation {
			Keep,                /**< Keep existing stencil data. */
			Zero,                /**< Set stencil data to zero. */
			Replace,             /**< Set the data to the defined reference value. */
			Increment,           /**< Increment the stencil data by 1. */
			Decrement,           /**< Decrement the stencil data by 1. */
			IncrementSaturation, /**< Increment the stencil data by 1 and clamp the result. */
			DecrementSaturation, /**< Decrement the stencil data by 1 and clamp the result. */
			Invert               /**< Invert the stencil data. */
		};
	};

	struct DepthStencilDesc {
		bool depthEnable;                    /**< Enable or disable depth testing. */
		bool depthWriteMask;                 /**< Enable or disable writing to the depth buffer. */
		CompareFunction::Function depthFunc; /**< Function for comparing depth data against existing depth data. */
		//
		bool stencilEnable;   /**< Enable or disable stencil testing. */
		int stencilReadMask;  /**< Mask value for reading stencil data. */
		int stencilWriteMask; /**< Mask value for writing stencil data. */
		//
		StencilOperation::Operation frontStencilFailOp;      /**< Operation when stencil testing fails. */
		StencilOperation::Operation frontStencilDepthFailOp; /**< Operation when stencil testing passes but depth testing fails. */
		StencilOperation::Operation frontStencilPassOp;      /**< Operation when stencil testing and depth testing both pass. */
		CompareFunction::Function frontStencilFunc;          /**< Function for comparing stencil data against existing stencil data. */
		//
		StencilOperation::Operation backStencilFailOp;      /**< Operation when stencil testing fails. */
		StencilOperation::Operation backStencilDepthFailOp; /**< Operation when stencil testing passes but depth testing fails. */
		StencilOperation::Operation backStencilPassOp;      /**< Operation when stencil testing and depth testing both pass. */
		CompareFunction::Function backStencilFunc;          /**< Function for comparing stencil data against existing stencil data. */
		//
		bool twoSidedStencil; /**< If two sided, front and back stenciling are separated; otherwise, both are set to front. */
		int stencilRef;       /**< Reference value for comparison with stenciling. */

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

		/**
		 * Uninitializes the depth stencil state.
		 */
		virtual void destroy()=0;
	};
} // ciri

#endif /* __ciri_idepthstencilstate__ */
