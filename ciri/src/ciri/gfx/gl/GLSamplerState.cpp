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

		glSamplerParameteri(_samplerId, GL_TEXTURE_WRAP_S, ciriToGlWrap(desc.wrapU));
		glSamplerParameteri(_samplerId, GL_TEXTURE_WRAP_T, ciriToGlWrap(desc.wrapV));
		glSamplerParameteri(_samplerId, GL_TEXTURE_WRAP_R, ciriToGlWrap(desc.wrapW));

		GLint filterMin;
		GLint filterMax;
		ciriToGlFilter(desc.filter, &filterMin, &filterMax);
		glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER, filterMin);
		glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, filterMax);

		GLfloat maxAniso;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
		const float aniso = cc::math::clamp<float>(static_cast<float>(desc.maxAnisotropy), 1.0f, maxAniso);
		glSamplerParameterf(_samplerId, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso); // todo: if supported??

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
				return GL_REPEAT; // dx has no fail, so match its default return type
			}
		}
	}

	void GLSamplerState::ciriToGlFilter( SamplerFilter::Mode mode, GLint* outMin, GLint* outMag ) const {
		// note: mag can only be nearest or linear; min controls everything else
		// https://www.opengl.org/wiki/Sampler_Object#Filtering

		switch( mode ) {
			case SamplerFilter::Point: {
				*outMin = GL_NEAREST;
				*outMag = GL_NEAREST;
				break;
			}

			case SamplerFilter::PointLinear: {
				*outMin = GL_NEAREST;
				*outMag = GL_LINEAR;
				break;
			}

			case SamplerFilter::LinearPoint: {
				*outMin = GL_LINEAR;
				*outMag = GL_NEAREST;
				break;
			}

			case SamplerFilter::Bilinear: {
				*outMin = GL_LINEAR;
				*outMag = GL_LINEAR;
				break;
			}

			case SamplerFilter::Trilinear: {
				*outMin = GL_LINEAR_MIPMAP_LINEAR;
				*outMag = GL_LINEAR;
				break;
			}

			case SamplerFilter::Anisotropic: {
				*outMin = GL_LINEAR_MIPMAP_LINEAR;
				*outMag = GL_LINEAR;
				break;
			}

			default: {
				*outMin = GL_NEAREST;
				*outMag = GL_NEAREST; // point filtering by default
				break;
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
				return GL_NEVER; // dx has no fail, so match it
			}
		}
	}
} // ciri