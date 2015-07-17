#ifndef __ciri_glconstantbuffer__
#define __ciri_glconstantbuffer__

#include "../IConstantBuffer.hpp"

namespace ciri {
	class GLConstantBuffer : public IConstantBuffer {
	public:
		GLConstantBuffer();
		virtual ~GLConstantBuffer();

		virtual err::ErrorCode initialize();
		virtual void destroy();
	};
} // ciri

#endif /* __ciri_glconstantbuffer__ */