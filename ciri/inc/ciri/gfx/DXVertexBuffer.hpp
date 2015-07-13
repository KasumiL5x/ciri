#ifndef __ciri_dxvertexbuffer__
#define __ciri_dxvertexbuffer__

#include <d3d11_1.h>
#include "IVertexBuffer.hpp"

namespace ciri {
	class DXGraphicsDevice;

	class DXVertexBuffer : public IVertexBuffer {
	public:
		DXVertexBuffer( DXGraphicsDevice* device );
		virtual ~DXVertexBuffer();

		virtual bool set( void* vertices, int vertexStride, int vertexCount, bool dynamic );
		virtual void destroy();
		virtual int getStride() const;

		ID3D11Buffer* getVertexBuffer() const;

	private:
		DXGraphicsDevice* _device;
		ID3D11Buffer* _vertexBuffer;
		int _vertexStride;
	};
} // ciri

#endif /* __ciri_dxvertexbuffer__ */