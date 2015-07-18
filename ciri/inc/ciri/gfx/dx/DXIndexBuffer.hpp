#ifndef __ciri_dxindexbuffer__
#define __ciri_dxindexbuffer__

#include "../IIndexBuffer.hpp"

namespace ciri {
	class DXIndexBuffer : public IIndexBuffer {
	public:
		DXIndexBuffer();
		virtual ~DXIndexBuffer();

		virtual void destroy();
	};
} // ciri

#endif /* __ciri_dxindexbuffer__ */