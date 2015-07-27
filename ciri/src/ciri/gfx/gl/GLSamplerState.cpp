#include <ciri/gfx/gl/GLSamplerState.hpp>

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

		glSamplerParameteri(_samplerId, GL_TEXTURE_WRAP_S, ciriToGlWrap(desc.wrapU));
		glSamplerParameteri(_samplerId, GL_TEXTURE_WRAP_T, ciriToGlWrap(desc.wrapV));
		glSamplerParameteri(_samplerId, GL_TEXTURE_WRAP_R, ciriToGlWrap(desc.wrapW));

		glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER, ciriToGlFilter(desc.filterMin));
		glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, ciriToGlFilter(desc.filterMax));

		glSamplerParameterfv(_samplerId, GL_TEXTURE_BORDER_COLOR, &desc.borderColor[0]);

		glSamplerParameterf(_samplerId, GL_TEXTURE_MIN_LOD, desc.minLod);
		glSamplerParameterf(_samplerId, GL_TEXTURE_MAX_LOD, desc.maxLod);
		glSamplerParameterf(_samplerId, GL_TEXTURE_LOD_BIAS, desc.lodBias);

		glSamplerParameteri(_samplerId, GL_TEXTURE_COMPARE_FUNC, ciriToClFunc(desc.comparisonFunc));

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
				return GL_INVALID_ENUM;
			}
		}
	}

	GLint GLSamplerState::ciriToGlFilter( SamplerFilter::Mode mode ) const {
		switch( mode ) {
			case SamplerFilter::Nearest: {
				return GL_NEAREST;
			}

			case SamplerFilter::Linear: {
				return GL_LINEAR;
			}

			case SamplerFilter::Bilinear: {
				return GL_LINEAR; // todo: from gamedev.net, is it right?
			}

			case SamplerFilter::Trilinear: {
				return GL_LINEAR_MIPMAP_LINEAR; // todo: from gamedev.net, is it right?
			}

			case SamplerFilter::Anisotropic: {
				return GL_LINEAR_MIPMAP_LINEAR; // todo: i'm literally guessing
			}

			default: {
				return GL_INVALID_ENUM;
			}
		}
	}

	GLint GLSamplerState::ciriToClFunc( SamplerComparison::Mode mode ) const {
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
				return GL_INVALID_ENUM;
			}
		}
	}
} // ciri