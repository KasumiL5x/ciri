#ifndef __ciri_dxvertexbuffer__
#define __ciri_dxvertexbuffer__

#include "IVertexBuffer.hpp"

namespace ciri {
	class DXVertexBuffer : public IVertexBuffer {
	public:
		DXVertexBuffer();
		virtual ~DXVertexBuffer();
	};
} // ciri

#endif /* __ciri_dxvertexbuffer__ */