#ifndef __ciri_glindexbuffer__
#define __ciri_glindexbuffer__

#include <gl/glew.h>
#include "../IIndexBuffer.hpp"

namespace ciri {
	class GLIndexBuffer : public IIndexBuffer {
	public:
		GLIndexBuffer();
		virtual ~GLIndexBuffer();

		virtual err::ErrorCode set( int* indices, int indexCount, bool dynamic );
		virtual void destroy();
		virtual int getIndexCount() const;

		GLuint getEvbo() const;

	private:
		GLuint _evbo;
		int _indexCount;
	};
} // ciri

#endif /* __ciri_glindexbuffer__ */