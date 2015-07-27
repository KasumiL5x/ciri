#ifndef __ciri_glsamplerstate__
#define __ciri_glsamplerstate__

#include <gl/glew.h>
#include "../ISamplerState.hpp"

namespace ciri {
	class GLSamplerState : public ISamplerState {
	public:
		GLSamplerState();
		virtual ~GLSamplerState();

		bool create( const SamplerDesc& desc );
		virtual void destroy();

		GLuint getSamplerId() const;

	private:
		GLint ciriToGlWrap( SamplerWrap::Mode mode ) const;
		GLint ciriToGlFilter( SamplerFilter::Mode mode ) const;
		GLint ciriToClFunc( SamplerComparison::Mode mode ) const;

	private:
		GLuint _samplerId;
	};
}

#endif /* __ciri_glsamplerstate__ */