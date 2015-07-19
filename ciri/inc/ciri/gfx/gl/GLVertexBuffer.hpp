#ifndef __ciri_glvertexbuffer__
#define __ciri_glvertexbuffer__

#include <gl/glew.h>
#include "../IVertexBuffer.hpp"

namespace ciri {
class GLVertexBuffer : public IVertexBuffer {
	public:
		GLVertexBuffer();
		virtual ~GLVertexBuffer();

		virtual bool set( void* vertices, int vertexStride, int vertexCount, bool dynamic );
		virtual void destroy();
		virtual int getStride() const;

		GLuint getVbo() const;

	private:
		GLuint _vbo;
		int _vertexStride;
	};
} // ciri

#endif /* __ciri_glvertexbuffer__ */