#ifndef __ciri_gfx_dxvertexbuffer__
#define __ciri_gfx_dxvertexbuffer__

#include <memory>
#include <d3d11.h>
#include <ciri/gfx/IVertexBuffer.hpp>

namespace ciri {
	class DXGraphicsDevice;

	class DXVertexBuffer : public IVertexBuffer {
	public:
		DXVertexBuffer( const std::shared_ptr<DXGraphicsDevice>& device );
		virtual ~DXVertexBuffer();

		virtual ErrorCode set( void* vertices, int vertexStride, int vertexCount, bool dynamic ) override;
		virtual void destroy() override;
		virtual int getStride() const override;
		virtual int getVertexCount() override;

		ID3D11Buffer* getVertexBuffer() const;

	private:
		ErrorCode createBuffer( void* vertices, int vertexStride, int vertexCount, bool dynamic );
		ErrorCode updateBuffer( void* vertices, int vertexStride, int vertexCount );

	private:
		std::shared_ptr<DXGraphicsDevice> _device;
		ID3D11Buffer* _vertexBuffer;
		int _vertexStride;
		int _vertexCount;
		bool _isDynamic;
	};
} // ciri

#endif /* __ciri_gfx_dxvertexbuffer__ */
