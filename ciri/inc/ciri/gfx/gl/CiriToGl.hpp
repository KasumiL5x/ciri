#ifndef __ciri_ciritogl__
#define __ciri_ciritogl__

#include <gl/glew.h>
#include "../CompareFunction.hpp"
#include "../StencilOperation.hpp"
#include "../SamplerWrap.hpp"
#include "../SamplerFilter.hpp"

namespace ciri {
	static GLenum ciriToGlFunc( CompareFunction::Function func ) {
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
					return GL_NEVER;
			}
		}
	}

	static GLenum ciriToGlOp( const StencilOperation::Operation op ) {
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
				return GL_KEEP;
			}
		}
	}

	static GLint ciriToGlWrap( SamplerWrap::Mode mode ) {
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
				return GL_REPEAT;
			}
		}
	}

	static void ciriToGlFilter( SamplerFilter::Filter mode, GLint* outMin, GLint* outMag, bool mipmaps ) {
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
} // ciri

#endif /* __ciri_ciritogl__ */