#ifndef __ciri_glindexbuffer__
#define __ciri_glindexbuffer__

#include <gl/glew.h>
#include "../IIndexBuffer.hpp"

namespace ciri {
	class GLIndexBuffer : public IIndexBuffer {
	public:
		GLIndexBuffer();
		virtual ~GLIndexBuffer();

		virtual bool set( int* indices, int indexCount, bool dynamic );
		virtual void destroy();

		GLuint getEvbo() const;

	private:
		GLuint _evbo;
	};
} // ciri

#endif /* __ciri_glindexbuffer__ */