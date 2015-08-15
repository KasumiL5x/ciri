#ifndef __ciri_checkglerror__
#define __ciri_checkglerror__

#include <gl/glew.h>
#include "../../util/Log.hpp"

namespace ciri {
	static __forceinline bool checkGlError() {
#ifdef _DEBUG
		const GLenum err = glGetError();
		if( GL_NO_ERROR != err ) {
			const char* errStr = reinterpret_cast<const char*>(glewGetErrorString(err));
			Logs::get(Logs::Debug).printError(std::string(errStr));
			return false;
		}
		return true;
#else
		return true;
#endif
	}
}

#endif /* __ciri_checkglerror__ */