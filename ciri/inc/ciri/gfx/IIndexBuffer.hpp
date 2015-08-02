#ifndef __ciri_iindexbuffer__
#define __ciri_iindexbuffer__

#include "../ErrorCodes.hpp"

namespace ciri {
	class IIndexBuffer {
	public:
		IIndexBuffer() {
		}
		virtual ~IIndexBuffer() {
		}

		virtual err::ErrorCode set( int* indices, int indexCount, bool dynamic )=0;
		virtual void destroy()=0;
		virtual int getIndexCount() const=0;
	};
} // ciri

#endif /* __ciri_iindexbuffer__ */