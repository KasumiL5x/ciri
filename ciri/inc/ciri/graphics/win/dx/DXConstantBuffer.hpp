#ifndef __ciri_graphics_DXConstantBuffer__
#define __ciri_graphics_DXConstantBuffer__

#include <memory>
#include <d3d11.h>
#include <ciri/graphics/IConstantBuffer.hpp>

namespace ciri { namespace graphics {

class DXGraphicsDevice;

class DXConstantBuffer : public IConstantBuffer {
public:
	DXConstantBuffer( const std::shared_ptr<DXGraphicsDevice>& device );
	virtual ~DXConstantBuffer();

	virtual core::ErrorCode setData( int dataSize, void* data ) override;
	virtual void destroy() override;

	ID3D11Buffer* getBuffer() const;

	void setIndex( int val );
	int getIndex() const;

private:
	std::shared_ptr<DXGraphicsDevice> _device;
	ID3D11Buffer* _buffer;
	int _index;
};

}}

#endif