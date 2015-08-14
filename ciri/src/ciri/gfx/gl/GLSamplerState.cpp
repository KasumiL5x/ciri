#include <ciri/gfx/gl/GLSamplerState.hpp>
#include <cc/Common.hpp>

namespace ciri {
	GLSamplerState::GLSamplerState()
		: ISamplerState(), _samplerId(0) {
	}

	GLSamplerState::~GLSamplerState() {
		destroy();
	}

	bool GLSamplerState::create( const SamplerDesc& desc ) {
		if( _samplerId != 0 ) {
			return false;
		}

		glGenSamplers(1, &_samplerId);

		GLint filterMin;
		GLint filterMag;
		ciriToGlFilter(desc.filter, &filterMin, &filterMag, desc.useMipmaps);
		glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER, filterMin);
		glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, filterMag);

		GLfloat maxAnisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
		const float anisotropy = (SamplerFilter::Anisotropic == desc.filter) ? cc::math::clamp(static_cast<float>(desc.maxAnisotropy), 1.0f, maxAnisotropy) : 1.0f;
		glSamplerParameterf(_samplerId, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);

		glSamplerParameteri(_samplerId, GL_TEXTURE_WRAP_S, ciriToGlWrap(desc.wrapU));
		glSamplerParameteri(_samplerId, GL_TEXTURE_WRAP_T, ciriToGlWrap(desc.wrapV));
		glSamplerParameteri(_samplerId, GL_TEXTURE_WRAP_R, ciriToGlWrap(desc.wrapW));

		glSamplerParameterfv(_samplerId, GL_TEXTURE_BORDER_COLOR, &desc.borderColor[0]);

		glSamplerParameterf(_samplerId, GL_TEXTURE_MIN_LOD, desc.useMipmaps ? desc.minLod : 0.0f);
		glSamplerParameterf(_samplerId, GL_TEXTURE_MAX_LOD, desc.useMipmaps ? desc.maxLod : 0.0f);
		glSamplerParameterf(_samplerId, GL_TEXTURE_LOD_BIAS, desc.lodBias);

		if( desc.comparisonFunc != SamplerComparison::Never ) {
			glSamplerParameteri(_samplerId, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
			glSamplerParameteri(_samplerId, GL_TEXTURE_COMPARE_FUNC, ciriToGlFunc(desc.comparisonFunc));
		} else {
			glSamplerParameteri(_samplerId, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		}

		return true;
	}

	void GLSamplerState::destroy() {
		if( _samplerId != 0 ) {
			glDeleteSamplers(1, &_samplerId);
			_samplerId = 0;
		}
	}

	GLuint GLSamplerState::getSamplerId() const {
		return _samplerId;
	}

	GLint GLSamplerState::ciriToGlWrap( SamplerWrap::Mode mode ) const {
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

	void GLSamplerState::ciriToGlFilter( SamplerFilter::Mode mode, GLint* outMin, GLint* outMag, bool mipmaps ) const {
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

	GLint GLSamplerState::ciriToGlFunc( SamplerComparison::Mode mode ) const {
		switch( mode ) {
			case SamplerComparison::Never: {
				return GL_NEVER;
			}

			case SamplerComparison::Always: {
				return GL_ALWAYS;
			}

			case SamplerComparison::Less: {
				return GL_LESS;
			}

			case SamplerComparison::Equal: {
				return GL_EQUAL;
			}

			case SamplerComparison::LessEqual: {
				return GL_LEQUAL;
			}

			case SamplerComparison::Greater: {
				return GL_GREATER;
			}

			case SamplerComparison::GreaterEqual: {
				return GL_GEQUAL;
			}

			case SamplerComparison::NotEqual: {
				return GL_NOTEQUAL;
			}

			default: {
				return GL_NEVER;
			}
		}
	}
} // ciri