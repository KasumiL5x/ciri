#ifndef __ciri_graphics_CheckGLError__
#define __ciri_graphics_CheckGLError__

#include <GL/glew.h>
#include <ciri/core/Log.hpp>

namespace ciri {

static __forceinline bool checkGlError() {
#ifdef _DEBUG
	const GLenum err = glGetError();
	if( GL_NO_ERROR != err ) {
		const char* errStr = reinterpret_cast<const char*>(glewGetErrorString(err));
		ciri::Logs::get(ciri::Logs::Debug).printError(std::string(errStr));
		return false;
	}
	return true;
#else
	return true;
#endif
}

}

#endif