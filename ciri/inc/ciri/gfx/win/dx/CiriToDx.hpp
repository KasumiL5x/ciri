#ifndef __ciri_gfx_ciritodx__
#define __ciri_gfx_ciritodx__

#include <d3d11.h>
#include <ciri/gfx/CompareFunction.hpp>
#include <ciri/gfx/StencilOperation.hpp>
#include <ciri/gfx/SamplerWrap.hpp>
#include <ciri/gfx/SamplerFilter.hpp>
#include <ciri/gfx/TextureFormat.hpp>
#include <ciri/gfx/VertexFormat.hpp>
#include <ciri/gfx/PrimitiveTopology.hpp>
#include <ciri/gfx/BlendMode.hpp>
#include <ciri/gfx/BlendFunction.hpp>
#include <ciri/gfx/BlendColorMask.hpp>

namespace ciri {
	static D3D11_COMPARISON_FUNC ciriToDxComparisonFunc( CompareFunction func ) {
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
				throw; //return D3D11_COMPARISON_NEVER;
			}
		}
	}

	static D3D11_STENCIL_OP ciriToDxStencilOp( StencilOperation op ) {
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
				throw; //return D3D11_STENCIL_OP_KEEP;
			}
		}
	}

	static D3D11_TEXTURE_ADDRESS_MODE ciriToDxSamplerWrap( SamplerWrap mode ) {
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
				throw; //return D3D11_TEXTURE_ADDRESS_WRAP;
			}
		}
	}

	static D3D11_FILTER ciriToDxSamplerFilter( SamplerFilter mode, bool comparison ) {
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
			case TextureFormat::RGBA32_UINT: {
				return DXGI_FORMAT_R8G8B8A8_UNORM;
			}

			case TextureFormat::RGBA32_Float: {
				return DXGI_FORMAT_R32G32B32A32_FLOAT;
			}

			default: {
				throw; //return DXGI_FORMAT_UNKNOWN;
			}
		}
	}

	static DXGI_FORMAT ciriToDxVertexFormat( VertexFormat format ) {
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
				throw; //return DXGI_FORMAT_UNKNOWN;
			}
		}
	}

	static D3D_PRIMITIVE_TOPOLOGY ciriToDxTopology( PrimitiveTopology topology ) {
		switch( topology ) {
			case PrimitiveTopology::PointList: {
				return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
			}

			case PrimitiveTopology::LineList: {
				return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
			}

			case PrimitiveTopology::LineStrip: {
				return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
			}

			case PrimitiveTopology::TriangleList: {
				return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			}

			case PrimitiveTopology::TriangleStrip: {
				return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			}

			default: {
				throw; //return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
			}
		}
	}

	static D3D11_BLEND ciriToDxBlendMode( BlendMode mode, bool alpha ) {
		switch( mode ) {
			case BlendMode::One: {
				return D3D11_BLEND_ONE;
			}

			case BlendMode::Zero: {
				return D3D11_BLEND_ZERO;
			}

			case BlendMode::SourceColor: {
				return alpha ? D3D11_BLEND_SRC_ALPHA : D3D11_BLEND_SRC_COLOR;
			}

			case BlendMode::InverseSourceColor: {
					return alpha ? D3D11_BLEND_INV_SRC_ALPHA : D3D11_BLEND_INV_SRC_COLOR;
			}

			case BlendMode::SourceAlpha: {
				return D3D11_BLEND_SRC_ALPHA;
			}

			case BlendMode::InverseSourceAlpha: {
				return D3D11_BLEND_INV_SRC_ALPHA;
			}

			case BlendMode::DestinationColor: {
				return alpha ? D3D11_BLEND_DEST_ALPHA : D3D11_BLEND_DEST_COLOR;
			}

			case BlendMode::InverseDestinationColor: {
				return alpha ? D3D11_BLEND_INV_DEST_ALPHA : D3D11_BLEND_INV_DEST_COLOR;
			}

			case BlendMode::DestinationAlpha: {
				return D3D11_BLEND_DEST_ALPHA;
			}

			case BlendMode::InverseDestinationAlpha: {
				return D3D11_BLEND_INV_DEST_ALPHA;
			}

			case BlendMode::BlendFactor: {
				return D3D11_BLEND_BLEND_FACTOR;
			}

			case BlendMode::InverseBlendFactor: {
				return D3D11_BLEND_INV_BLEND_FACTOR;
			}

			case BlendMode::SourceAlphaSaturation: {
				return D3D11_BLEND_SRC_ALPHA_SAT;
			}

			default: {
				throw;
			}
		}
	}

	static D3D11_BLEND_OP ciriToDxBlendFunction( BlendFunction func ) {
		switch( func ) {
			case BlendFunction::Add: {
				return D3D11_BLEND_OP_ADD;
			}

			case BlendFunction::Subtract: {
				return D3D11_BLEND_OP_SUBTRACT;
			}

			case BlendFunction::ReverseSubtract: {
				return D3D11_BLEND_OP_REV_SUBTRACT;
			}

			case BlendFunction::Min: {
				return D3D11_BLEND_OP_MIN;
			}

			case BlendFunction::Max: {
				return D3D11_BLEND_OP_MAX;
			}
			default: {
				throw;
			}
		}
	}

	static UINT8 ciriToDxBlendColorMask( int mask ) {
		UINT8 val = 0;

		if( mask & static_cast<int>(BlendColorMask::Red) ) {
			val |= D3D11_COLOR_WRITE_ENABLE_RED;
		}

		if( mask & static_cast<int>(BlendColorMask::Green) ) {
			val |= D3D11_COLOR_WRITE_ENABLE_GREEN;
		}

		if( mask & static_cast<int>(BlendColorMask::Blue) ) {
			val |= D3D11_COLOR_WRITE_ENABLE_BLUE;
		}

		if( mask & static_cast<int>(BlendColorMask::Alpha) ) {
			val |= D3D11_COLOR_WRITE_ENABLE_ALPHA;
		}

		return val;
	}
} // ciri

#endif /* __ciri_gfx_ciritodx__ */
