#ifndef __ciri_gfx_dxconstantbuffer__
#define __ciri_gfx_dxconstantbuffer__

#include <memory>
#include <d3d11.h>
#include <ciri/gfx/IConstantBuffer.hpp>

namespace ciri {
	class DXGraphicsDevice;

	class DXConstantBuffer : public IConstantBuffer {
	public:
		DXConstantBuffer( const std::shared_ptr<DXGraphicsDevice>& device );
		virtual ~DXConstantBuffer();

		virtual ErrorCode setData( int dataSize, void* data ) override;
		virtual void destroy() override;

		ID3D11Buffer* getBuffer() const;

		void setIndex( int val );
		int getIndex() const;

	private:
		std::shared_ptr<DXGraphicsDevice> _device;
		ID3D11Buffer* _buffer;
		int _index;
	};
} // ciri

#endif /* __ciri_gfx_dxconstantbuffer__ */
