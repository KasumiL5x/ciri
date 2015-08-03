#ifndef __ciri_glconstantbuffer__
#define __ciri_glconstantbuffer__

#include <gl/glew.h>
#include "../IConstantBuffer.hpp"

namespace ciri {
	class GLConstantBuffer : public IConstantBuffer {
	public:
		GLConstantBuffer( GLuint index );
		virtual ~GLConstantBuffer();

		virtual err::ErrorCode setData( int dataSize, void* data );
		virtual void destroy();

		GLuint getUbo() const;
		GLuint getIndex() const;

	private:
		GLuint _ubo;
		GLuint _index;
	};
} // ciri

#endif /* __ciri_glconstantbuffer__ */