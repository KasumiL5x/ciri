#ifndef __ciri_dxconstantbuffer__
#define __ciri_dxconstantbuffer__

#include "../IConstantBuffer.hpp"
#include <d3d11_1.h>

namespace ciri {
	class DXGraphicsDevice;

	class DXConstantBuffer : public IConstantBuffer {
	public:
		DXConstantBuffer( DXGraphicsDevice* device );
		virtual ~DXConstantBuffer();

		virtual err::ErrorCode setData( int dataSize, void* data );
		virtual void destroy();

		ID3D11Buffer* getBuffer() const;

		void setIndex( int val );
		int getIndex() const;

	private:
		DXGraphicsDevice* _device;
		ID3D11Buffer* _buffer;
		int _index;
	};
} // ciri

#endif /* __ciri_dxconstantbuffer__ */