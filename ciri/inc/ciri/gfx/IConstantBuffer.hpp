#ifndef __ciri_iconstantbuffer__
#define __ciri_iconstantbuffer__

#include <ciri/ErrorCodes.hpp>

namespace ciri {
	class IConstantBuffer {
	public:
		IConstantBuffer() {
		}

		virtual ~IConstantBuffer() {
		}

		virtual err::ErrorCode initialize()=0;
		virtual void destroy()=0;
	};
} // ciri

#endif /* __ciri_iconstantbuffer__ */