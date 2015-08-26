#ifndef __ciri_ciritodx__
#define __ciri_ciritodx__

#include <d3d11.h>
#include "../CompareFunction.hpp"
#include "../StencilOperation.hpp"
#include "../SamplerWrap.hpp"
#include "../SamplerFilter.hpp"
#include "../TextureFormat.hpp"
#include "../VertexFormat.hpp"

namespace ciri {
	static D3D11_COMPARISON_FUNC ciriToDxComparisonFunc( CompareFunction::Function func ) {
		switch( func ) {
			case CompareFunction::Never: {
				return D3D11_COMPARISON_NEVER;
			}

			case CompareFunction::Always: {
				return D3D11_COMPARISON_ALWAYS;
			}

			case CompareFunction::Less: {
				return D3D11_COMPARISON_LESS;
			}

			case CompareFunction::Equal: {
				return D3D11_COMPARISON_EQUAL;
			}

			case CompareFunction::LessEqual: {
				return D3D11_COMPARISON_LESS_EQUAL;
			}

			case CompareFunction::Greater: {
				return D3D11_COMPARISON_GREATER;
			}

			case CompareFunction::GreaterEqual: {
				return D3D11_COMPARISON_GREATER_EQUAL;
			}

			case CompareFunction::NotEqual: {
				return D3D11_COMPARISON_NOT_EQUAL;
			}

			default: {
				return D3D11_COMPARISON_NEVER;
			}
		}
	}

	static D3D11_STENCIL_OP ciriToDxStencilOp( StencilOperation::Operation op ) {
		switch( op ) {
			case StencilOperation::Keep: {
				return D3D11_STENCIL_OP_KEEP;
			}
			case StencilOperation::Zero: {
				return D3D11_STENCIL_OP_ZERO;
			}
			case StencilOperation::Replace: {
				return D3D11_STENCIL_OP_REPLACE;
			}
			case StencilOperation::Increment: {
				return D3D11_STENCIL_OP_INCR;
			}
			case StencilOperation::Decrement: {
				return D3D11_STENCIL_OP_DECR;
			}
			case StencilOperation::IncrementSaturation: {
				return D3D11_STENCIL_OP_INCR_SAT;
			}
			case StencilOperation::DecrementSaturation: {
				return D3D11_STENCIL_OP_DECR_SAT;
			}
			case StencilOperation::Invert: {
				return D3D11_STENCIL_OP_INVERT;
			}
			default: {
				return D3D11_STENCIL_OP_KEEP;
			}
		}
	}

	static D3D11_TEXTURE_ADDRESS_MODE ciriToDxSamplerWrap( SamplerWrap::Mode mode ) {
		switch( mode ) {
			case SamplerWrap::Wrap: {
				return D3D11_TEXTURE_ADDRESS_WRAP;
			}

			case SamplerWrap::Mirror: {
				return D3D11_TEXTURE_ADDRESS_MIRROR;
			}

			case SamplerWrap::Clamp: {
				return D3D11_TEXTURE_ADDRESS_CLAMP;
			}

			case SamplerWrap::Border: {
				return D3D11_TEXTURE_ADDRESS_BORDER;
			}

			case SamplerWrap::MirrorOnce: {
				return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
			}

			default: {
				return D3D11_TEXTURE_ADDRESS_WRAP;
			}
		}
	}

	static D3D11_FILTER ciriToDxSamplerFilter( SamplerFilter::Filter mode, bool comparison ) {
		int filter = 0;
		switch( mode ) {
			case SamplerFilter::Point: {
				filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
				break;
			}
			case SamplerFilter::Linear: {
				filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				break;
			}
			case SamplerFilter::Anisotropic: {
				filter = D3D11_FILTER_ANISOTROPIC;
				break;
			}
			case SamplerFilter::PointMipLinear: {
					filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
				break;
			}
			case SamplerFilter::LinearMipPoint: {
				filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
				break;
			}
			case SamplerFilter::MinLinearMagPointMipLinear: {
				filter = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
				break;
			}
			case SamplerFilter::MinLinearMagPointMipPoint: {
				filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
				break;
			}
			case SamplerFilter::MinPointMagLinearMipLinear: {
				filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
				break;
			}
			case SamplerFilter::MinPointMagLinearMipPoint: {
				filter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
				break;
			}
		}

		// thanks, http://www.humus.name/!
		if( comparison ) {
			filter |= 0x80;
		}

		return (D3D11_FILTER)filter;
	}

	static DXGI_FORMAT ciriToDxTextureFormat( TextureFormat::Format format ) {
		switch( format ) {
			case TextureFormat::Color: {
				return DXGI_FORMAT_R8G8B8A8_UNORM;
			}

			case TextureFormat::RGB32_Float: {
				return DXGI_FORMAT_R32G32B32_FLOAT; // todo: this breaks DX; do not support it (seems non XYZA formats for textures do), or work around it
			}

			case TextureFormat::RGBA32_Float: {
				return DXGI_FORMAT_R32G32B32A32_FLOAT;
			}

			default: {
				return DXGI_FORMAT_UNKNOWN;
			}
		}
	}

	static DXGI_FORMAT ciriToDxVertexFormat( VertexFormat::Format format ) {
		switch( format ) {
			case VertexFormat::Float: {
				return DXGI_FORMAT_R32_FLOAT;
			}

			case VertexFormat::Float2: {
				return DXGI_FORMAT_R32G32_FLOAT;
			}

			case VertexFormat::Float3: {
				return DXGI_FORMAT_R32G32B32_FLOAT;
			}

			case VertexFormat::Float4: {
				return DXGI_FORMAT_R32G32B32A32_FLOAT;
			}

			default: {
				return DXGI_FORMAT_UNKNOWN;
			}
		}
	}
} // ciri

#endif /* __ciri_ciritodx__ */