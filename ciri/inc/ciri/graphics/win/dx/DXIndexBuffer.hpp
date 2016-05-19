#ifndef __ciri_graphics_DXIndexBuffer__
#define __ciri_graphics_DXIndexBuffer__

#include <memory>
#include <d3d11.h>
#include <ciri/graphics/IIndexBuffer.hpp>

namespace ciri { namespace graphics {

class DXGraphicsDevice;

class DXIndexBuffer : public IIndexBuffer {
public:
	DXIndexBuffer( const std::shared_ptr<DXGraphicsDevice>& device );
	virtual ~DXIndexBuffer();

	virtual core::ErrorCode set( int* indices, int indexCount, bool dynamic ) override;
	virtual void destroy() override;
	virtual int getIndexCount() const override;

	ID3D11Buffer* getIndexBuffer() const;

private:
	std::shared_ptr<DXGraphicsDevice> _device;
	ID3D11Buffer* _indexBuffer;
	int _indexCount;
};

}}

#endif