#ifndef __ciri_gfx_glsamplerstate__
#define __ciri_gfx_glsamplerstate__

#include <gl/glew.h>
#include <ciri/gfx/ISamplerState.hpp>

namespace ciri {
	class GLSamplerState : public ISamplerState {
	public:
		GLSamplerState();
		virtual ~GLSamplerState();

		bool create( const SamplerDesc& desc );
		virtual void destroy() override;

		GLuint getSamplerId() const;

	private:
		GLuint _samplerId;
	};
} // ciri

#endif /* __ciri_gfx_glsamplerstate__ */
