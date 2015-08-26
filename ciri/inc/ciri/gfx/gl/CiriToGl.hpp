#ifndef __ciri_ciritogl__
#define __ciri_ciritogl__

#include <gl/glew.h>
#include "../CompareFunction.hpp"
#include "../StencilOperation.hpp"
#include "../SamplerWrap.hpp"
#include "../SamplerFilter.hpp"
#include "../TextureFormat.hpp"

namespace ciri {
	static GLenum ciriToGlComparisonFunc( CompareFunction::Function func ) {
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

	static GLenum ciriToGlStencilOp( const StencilOperation::Operation op ) {
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

	static GLint ciriToGlSamplerWrap( SamplerWrap::Mode mode ) {
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

	static void ciriToGlSamplerFilter( SamplerFilter::Filter mode, GLint* outMin, GLint* outMag, bool mipmaps ) {
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
		*internalFormat = GL_RGBA;
		*pixelFormat = GL_RGBA;
		*pixelType = GL_UNSIGNED_BYTE;

		switch( ciriFormat ) {
			case TextureFormat::Color: {
				*internalFormat = GL_RGBA;
				*pixelFormat = GL_RGBA;
				*pixelType = GL_UNSIGNED_BYTE;
				break;
			}

			case TextureFormat::RGB32_Float: {
				*internalFormat = GL_RGB32F;
				*pixelFormat = GL_RGB;
				*pixelType = GL_FLOAT;
				break;
			}

			case TextureFormat::RGBA32_Float: {
				*internalFormat = GL_RGBA32F;
				*pixelFormat = GL_RGBA;
				*pixelType = GL_FLOAT;
			}
		}
	}
} // ciri

#endif /* __ciri_ciritogl__ */