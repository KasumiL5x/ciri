#ifndef __ciri_dxconstantbuffer__
#define __ciri_dxconstantbuffer__

#include "../IConstantBuffer.hpp"

namespace ciri {
	class DXConstantBuffer : public IConstantBuffer {
	public:
		DXConstantBuffer();
		virtual ~DXConstantBuffer();

		virtual err::ErrorCode initialize();
		virtual void destroy();
	};
} // ciri

#endif /* __ciri_dxconstantbuffer__ */