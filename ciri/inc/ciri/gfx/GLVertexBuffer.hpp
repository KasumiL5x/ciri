#ifndef __ciri_glvertexbuffer__
#define __ciri_glvertexbuffer__

#include "IVertexBuffer.hpp"

namespace ciri {
	class GLVertexBuffer : public IVertexBuffer {
	public:
		GLVertexBuffer();
		virtual ~GLVertexBuffer();
	};
} // ciri

#endif /* __ciri_glvertexbuffer__ */