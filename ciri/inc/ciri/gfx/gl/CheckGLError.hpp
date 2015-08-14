#ifndef __ciri_checkglerror__
#define __ciri_checkglerror__

#include <gl/glew.h>

namespace ciri {
	static __forceinline bool checkGlError() {
#ifdef _DEBUG
		const GLenum err = glGetError();
		return (GL_NO_ERROR == err);
#else
		return true;
#endif
	}
}

#endif /* __ciri_checkglerror__ */