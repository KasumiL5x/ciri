#include <ciri/graphics/win/gl/GLSamplerState.hpp>
#include <ciri/graphics/win/gl/CiriToGl.hpp>
#include <cc/Common.hpp>

using namespace ciri;

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
	ciriToGlSamplerFilter(desc.filter, &filterMin, &filterMag, desc.useMipmaps);
	glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER, filterMin);
	glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, filterMag);

	GLfloat maxAnisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	const float anisotropy = (SamplerFilter::Anisotropic == desc.filter) ? cc::math::clamp(static_cast<float>(desc.maxAnisotropy), 1.0f, maxAnisotropy) : 1.0f;
	glSamplerParameterf(_samplerId, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);

	glSamplerParameteri(_samplerId, GL_TEXTURE_WRAP_S, ciriToGlSamplerWrap(desc.wrapU));
	glSamplerParameteri(_samplerId, GL_TEXTURE_WRAP_T, ciriToGlSamplerWrap(desc.wrapV));
	glSamplerParameteri(_samplerId, GL_TEXTURE_WRAP_R, ciriToGlSamplerWrap(desc.wrapW));

	glSamplerParameterfv(_samplerId, GL_TEXTURE_BORDER_COLOR, &desc.borderColor[0]);

	glSamplerParameterf(_samplerId, GL_TEXTURE_MIN_LOD, desc.useMipmaps ? desc.minLod : 0.0f);
	glSamplerParameterf(_samplerId, GL_TEXTURE_MAX_LOD, desc.useMipmaps ? desc.maxLod : 0.0f);
	glSamplerParameterf(_samplerId, GL_TEXTURE_LOD_BIAS, desc.lodBias);

	if( desc.comparisonFunc != CompareFunction::Never ) {
		glSamplerParameteri(_samplerId, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glSamplerParameteri(_samplerId, GL_TEXTURE_COMPARE_FUNC, ciriToGlComparisonFunc(desc.comparisonFunc));
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