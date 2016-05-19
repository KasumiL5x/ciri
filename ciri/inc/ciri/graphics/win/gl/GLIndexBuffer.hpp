#ifndef __ciri_graphics_GLIndexBuffer__
#define __ciri_graphics_GLIndexBuffer__

#include <GL/glew.h>
#include <ciri/graphics/IIndexBuffer.hpp>

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

}

#endif