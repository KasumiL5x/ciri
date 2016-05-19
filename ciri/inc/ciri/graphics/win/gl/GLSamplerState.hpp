#ifndef __ciri_graphics_GLSamplerState__
#define __ciri_graphics_GLSamplerState__

#include <GL/glew.h>
#include <ciri/graphics/ISamplerState.hpp>

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

}

#endif