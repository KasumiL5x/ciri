#ifndef __ciri_gfx_glconstantbuffer__
#define __ciri_gfx_glconstantbuffer__

#include <gl/glew.h>
#include <ciri/gfx/IConstantBuffer.hpp>

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
} // ciri

#endif /* __ciri_gfx_glconstantbuffer__ */
