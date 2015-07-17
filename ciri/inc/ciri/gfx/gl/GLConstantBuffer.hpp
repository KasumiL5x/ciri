#ifndef __ciri_glconstantbuffer__
#define __ciri_glconstantbuffer__

#include <gl/glew.h>
#include "../IConstantBuffer.hpp"

namespace ciri {
	class GLConstantBuffer : public IConstantBuffer {
	public:
		GLConstantBuffer();
		virtual ~GLConstantBuffer();

		virtual err::ErrorCode setData( int dataSize, void* data );
		virtual void destroy();

		GLuint getUbo() const;

	private:
		GLuint _ubo;
	};
} // ciri

#endif /* __ciri_glconstantbuffer__ */