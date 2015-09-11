#ifndef __ciri_gfx_glindexbuffer__
#define __ciri_gfx_glindexbuffer__

#include <gl/glew.h>
#include <ciri/gfx/IIndexBuffer.hpp>

namespace ciri {
	class GLIndexBuffer : public IIndexBuffer {
	public:
		GLIndexBuffer();
		virtual ~GLIndexBuffer();

		virtual ErrorCode set( int* indices, int indexCount, bool dynamic ) override;
		virtual void destroy() override;
		virtual int getIndexCount() const override;

		GLuint getEvbo() const;

	private:
		GLuint _evbo;
		int _indexCount;
	};
} // ciri

#endif /* __ciri_gfx_glindexbuffer__ */
