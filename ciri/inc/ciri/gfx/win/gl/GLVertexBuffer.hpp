#ifndef __ciri_gfx_glvertexbuffer__
#define __ciri_gfx_glvertexbuffer__

#include <gl/glew.h>
#include <ciri/gfx/IVertexBuffer.hpp>

namespace ciri {
class GLVertexBuffer : public IVertexBuffer {
	public:
		GLVertexBuffer();
		virtual ~GLVertexBuffer();

		virtual ErrorCode set( void* vertices, int vertexStride, int vertexCount, bool dynamic ) override;
		virtual void destroy() override;
		virtual int getStride() const override;
		virtual int getVertexCount() override;

		GLuint getVbo() const;

	private:
		GLuint _vbo;
		int _vertexStride;
		int _vertexCount;
		bool _isDynamic;
	};
} // ciri

#endif /* __ciri_gfx_glvertexbuffer__ */
