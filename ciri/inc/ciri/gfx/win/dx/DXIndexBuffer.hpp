#ifndef __ciri_gfx_dxindexbuffer__
#define __ciri_gfx_dxindexbuffer__

#include <d3d11.h>
#include <ciri/gfx/IIndexBuffer.hpp>

namespace ciri {
	class DXGraphicsDevice;

	class DXIndexBuffer : public IIndexBuffer {
	public:
		DXIndexBuffer( DXGraphicsDevice* device );
		virtual ~DXIndexBuffer();

		virtual ErrorCode set( int* indices, int indexCount, bool dynamic ) override;
		virtual void destroy() override;
		virtual int getIndexCount() const override;

		ID3D11Buffer* getIndexBuffer() const;

	private:
		DXGraphicsDevice* _device;
		ID3D11Buffer* _indexBuffer;
		int _indexCount;
	};
} // ciri

#endif /* __ciri_gfx_dxindexbuffer__ */
