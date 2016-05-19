#ifndef __ciri_graphics_GLVertexBuffer__
#define __ciri_graphics_GLVertexBuffer__

#include <GL/glew.h>
#include <ciri/graphics/IVertexBuffer.hpp>

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
	ErrorCode createBuffer( void* vertices, int vertexStride, int vertexCount, bool dynamic );
	ErrorCode updateBuffer( void* vertices, int vertexStride, int vertexCount );

private:
	GLuint _vbo;
	int _vertexStride;
	int _vertexCount;
	bool _isDynamic;
};

}

#endif