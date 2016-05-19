#ifndef __ciri_graphics_GLConstantBuffer__
#define __ciri_graphics_GLConstantBuffer__

#include <GL/glew.h>
#include <ciri/graphics/IConstantBuffer.hpp>

namespace ciri {

class GLConstantBuffer : public IConstantBuffer {
public:
	GLConstantBuffer( GLuint index );
	virtual ~GLConstantBuffer();

	virtual ErrorCode setData( int dataSize, void* data ) override;
	virtual void destroy() override;

	GLuint getUbo() const;
	GLuint getIndex() const;

private:
	GLuint _ubo;
	GLuint _index;
};

}

#endif