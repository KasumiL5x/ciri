#ifndef __ciri_graphics_DXDepthStencilState__
#define __ciri_graphics_DXDepthStencilState__

#include <memory>
#include <d3d11.h>
#include <ciri/graphics/IDepthStencilState.hpp>

namespace ciri { namespace graphics {

class DXGraphicsDevice;

class DXDepthStencilState : public IDepthStencilState {
public:
	DXDepthStencilState( const std::shared_ptr<DXGraphicsDevice>& device );
	virtual ~DXDepthStencilState();

	bool create( const DepthStencilDesc& desc );
	virtual void destroy() override;

	ID3D11DepthStencilState* getState();
	int getStencilRef() const;		

private:
	std::shared_ptr<DXGraphicsDevice> _device;
	ID3D11DepthStencilState* _state;
	DepthStencilDesc _desc;
};

}}

#endif