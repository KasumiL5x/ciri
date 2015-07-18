#ifndef __ciri_glindexbuffer__
#define __ciri_glindexbuffer__

#include "../IIndexBuffer.hpp"

namespace ciri {
	class GLIndexBuffer : public IIndexBuffer {
	public:
		GLIndexBuffer();
		virtual ~GLIndexBuffer();

		virtual void destroy();
	};
} // ciri

#endif /* __ciri_glindexbuffer__ */