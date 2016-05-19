#ifndef __ciri_graphics_CiriToGL__
#define __ciri_graphics_CiriToGL__

#include <GL/glew.h>
#include <ciri/graphics/CompareFunction.hpp>
#include <ciri/graphics/StencilOperation.hpp>
#include <ciri/graphics/SamplerWrap.hpp>
#include <ciri/graphics/SamplerFilter.hpp>
#include <ciri/graphics/TextureFormat.hpp>
#include <ciri/graphics/PrimitiveTopology.hpp>
#include <ciri/graphics/BlendMode.hpp>
#include <ciri/graphics/BlendFunction.hpp>

namespace ciri { namespace graphics {

static GLenum ciriToGlComparisonFunc( CompareFunction func ) {
	switch( func ) {
		case CompareFunction::Always: {
			return GL_ALWAYS;
		}
		case CompareFunction::Never: {
			return GL_NEVER;
		}
		case CompareFunction::Less: {
			return GL_LESS;
		}
		case CompareFunction::LessEqual: {
			return GL_LEQUAL;
		}
		case CompareFunction::Equal: {
			return GL_EQUAL;
		}
		case CompareFunction::GreaterEqual: {
			return GL_GEQUAL;
		}
		case CompareFunction::Greater: {
			return GL_GREATER;
		}
		case CompareFunction::NotEqual: {
			return GL_NOTEQUAL;
		}
		default: {
			throw; //return GL_NEVER;
		}
	}
}

static GLenum ciriToGlStencilOp( const StencilOperation op ) {
	switch( op ) {
		case StencilOperation::Keep: {
			return GL_KEEP;
		}
		case StencilOperation::Zero: {
			return GL_ZERO;
		}
		case StencilOperation::Replace: {
			return GL_REPLACE;
		}
		case StencilOperation::Increment: {
			return GL_INCR_WRAP;
		}
		case StencilOperation::Decrement: {
			return GL_DECR_WRAP;
		}
		case StencilOperation::IncrementSaturation: {
			return GL_INCR;
		}
		case StencilOperation::DecrementSaturation: {
			return GL_DECR;
		}
		case StencilOperation::Invert: {
			return GL_INVERT;
		}
		default: {
			throw; //return GL_KEEP;
		}
	}
}

static GLint ciriToGlSamplerWrap( SamplerWrap mode ) {
	switch( mode ) {
		case SamplerWrap::Wrap: {
			return GL_REPEAT;
		}

		case SamplerWrap::Mirror: {
			return GL_MIRRORED_REPEAT;
		}

		case SamplerWrap::Clamp: {
			return GL_CLAMP_TO_EDGE;
		}

		case SamplerWrap::Border: {
			return GL_CLAMP_TO_BORDER;
		}

		case SamplerWrap::MirrorOnce: {
			return GL_MIRROR_CLAMP_TO_EDGE;
		}

		default: {
			throw; //return GL_REPEAT;
		}
	}
}

static void ciriToGlSamplerFilter( SamplerFilter mode, GLint* outMin, GLint* outMag, bool mipmaps ) {
	// from monogame
	switch( mode ) {
		case SamplerFilter::Point: {
			*outMin = (mipmaps) ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
			*outMag = GL_NEAREST;
			break;
		}
		case SamplerFilter::Linear: {
			*outMin = (mipmaps) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
			*outMag = GL_LINEAR;
			break;
		}
		case SamplerFilter::Anisotropic: {
			*outMin = (mipmaps) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
			*outMag = GL_LINEAR;
			break;
		}
		case SamplerFilter::PointMipLinear: {
			*outMin = (mipmaps) ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST;
			*outMag = GL_NEAREST;
			break;
		}
		case SamplerFilter::LinearMipPoint: {
			*outMin = (mipmaps) ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;
			*outMag = GL_LINEAR;
			break;
		}
		case SamplerFilter::MinLinearMagPointMipLinear: {
			*outMin = (mipmaps) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
			*outMag = GL_NEAREST;
			break;
		}
		case SamplerFilter::MinLinearMagPointMipPoint: {
			*outMin = (mipmaps) ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;
			*outMag = GL_NEAREST;
			break;
		}
		case SamplerFilter::MinPointMagLinearMipLinear: {
			*outMin = (mipmaps) ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST;
			*outMag = GL_LINEAR;
			break;
		}
		case SamplerFilter::MinPointMagLinearMipPoint: {
			*outMin = (mipmaps) ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
			*outMag = GL_LINEAR;
			break;
		}
	}
}

static void ciriToGlTextureFormat( TextureFormat::Format ciriFormat, GLint* internalFormat, GLenum* pixelFormat, GLenum* pixelType ) {
	if( nullptr == internalFormat || nullptr == pixelFormat || nullptr == pixelType ) {
		return;
	}

	// http://www.opentk.com/files/doc/namespace_open_t_k_1_1_graphics_1_1_open_g_l.html#ae0f3f1e7b978e4937984b34fdebabf62a8f0fb883eb5a52838534191513e365a2
	// MonoGame.Framework.Graphics.GraphicsExtensions.cs: GetGLFormat
	//*internalFormat = GL_RGBA;
	//*pixelFormat = GL_RGBA;
	//*pixelType = GL_UNSIGNED_BYTE;

	switch( ciriFormat ) {
		case TextureFormat::RGBA32_UINT: {
			*internalFormat = GL_RGBA;
			*pixelFormat = GL_RGBA;
			*pixelType = GL_UNSIGNED_BYTE;
			break;
		}

		case TextureFormat::RGBA32_Float: {
			*internalFormat = GL_RGBA32F;
			*pixelFormat = GL_RGBA;
			*pixelType = GL_FLOAT;
			break;
		}

		case TextureFormat::R32_UINT: {
			*internalFormat = GL_RED;
			*pixelFormat = GL_RED;
			*pixelType = GL_UNSIGNED_BYTE;
			break;
		}

		default: {
			throw; // oh nooooooooooooooo! that's jeffry's dog!
		}
	}
}

static GLenum ciriToGlTopology( PrimitiveTopology topology ) {
	switch( topology ) {
		case PrimitiveTopology::PointList: {
			return GL_POINTS;
		}

		case PrimitiveTopology::LineList: {
			return GL_LINES;
		}

		case PrimitiveTopology::LineStrip: {
			return GL_LINE_STRIP;
		}

		case PrimitiveTopology::TriangleList: {
			return GL_TRIANGLES;
		}

		case PrimitiveTopology::TriangleStrip: {
			return GL_TRIANGLE_STRIP;
		}

		default: {
			throw; //return GL_INVALID_ENUM; // lol
		}
	}
}

static GLenum ciriToGlBlendMode( BlendMode mode, bool alpha ) {
	switch( mode ) {
		case BlendMode::One: {
			return GL_ONE;
		}

		case BlendMode::Zero: {
			return GL_ZERO;
		}

		case BlendMode::SourceColor: {
			return alpha ? GL_SRC_ALPHA : GL_SRC_COLOR;
		}

		case BlendMode::InverseSourceColor: {
				return alpha ? GL_ONE_MINUS_SRC_ALPHA : GL_ONE_MINUS_SRC_COLOR;
		}

		case BlendMode::SourceAlpha: {
			return GL_SRC_ALPHA;
		}

		case BlendMode::InverseSourceAlpha: {
			return GL_ONE_MINUS_SRC_ALPHA;
		}

		case BlendMode::DestinationColor: {
			return alpha ? GL_DST_ALPHA : GL_DST_COLOR;
		}

		case BlendMode::InverseDestinationColor: {
			return alpha ? GL_ONE_MINUS_DST_ALPHA : GL_ONE_MINUS_DST_COLOR;
		}

		case BlendMode::DestinationAlpha: {
			return GL_DST_ALPHA;
		}

		case BlendMode::InverseDestinationAlpha: {
			return GL_ONE_MINUS_DST_ALPHA;
		}

		case BlendMode::BlendFactor: {
			return GL_CONSTANT_COLOR; // todo: right, i think?
		}

		case BlendMode::InverseBlendFactor: {
			return GL_ONE_MINUS_CONSTANT_COLOR; // todo: right, i think?
		}

		case BlendMode::SourceAlphaSaturation: {
			return GL_SRC_ALPHA_SATURATE;
		}

		default: {
			throw;
		}
	}
}

static GLenum ciriToGlBlendFunction( BlendFunction func ) {
	switch( func ) {
		case BlendFunction::Add: {
			return GL_FUNC_ADD;
		}

		case BlendFunction::Subtract: {
			return GL_FUNC_SUBTRACT;
		}

		case BlendFunction::ReverseSubtract: {
			return GL_FUNC_REVERSE_SUBTRACT;
		}

		case BlendFunction::Min: {
			return GL_MIN;
		}

		case BlendFunction::Max: {
			return GL_MAX;
		}
		default: {
			throw;
		}
	}
}

}}

#endif