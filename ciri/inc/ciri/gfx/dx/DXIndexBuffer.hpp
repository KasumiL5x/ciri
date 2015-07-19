#ifndef __ciri_dxindexbuffer__
#define __ciri_dxindexbuffer__

#include <d3d11_1.h>
#include "../IIndexBuffer.hpp"

namespace ciri {
	class DXGraphicsDevice;

	class DXIndexBuffer : public IIndexBuffer {
	public:
		DXIndexBuffer( DXGraphicsDevice* device );
		virtual ~DXIndexBuffer();

		virtual bool set( int* indices, int indexCount, bool dynamic );
		virtual void destroy();
		virtual int getIndexCount() const;

		ID3D11Buffer* getIndexBuffer() const;

	private:
		DXGraphicsDevice* _device;
		ID3D11Buffer* _indexBuffer;
		int _indexCount;
	};
} // ciri

#endif /* __ciri_dxindexbuffer__ */