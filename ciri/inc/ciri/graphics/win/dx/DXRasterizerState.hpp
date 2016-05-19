#ifndef __ciri_graphics_DXRasterizerState__
#define __ciri_graphics_DXRasterizerState__

#include <memory>
#include <d3d11.h>
#include <ciri/graphics/IRasterizerState.hpp>

namespace ciri { namespace graphics {

class DXGraphicsDevice;

class DXRasterizerState : public IRasterizerState {
public:
	DXRasterizerState( const std::shared_ptr<DXGraphicsDevice>& device );
	virtual ~DXRasterizerState();

	bool create( const RasterizerDesc& desc );
	virtual void destroy() override;

	ID3D11RasterizerState* getRasterizerState() const;

private:
	std::shared_ptr<DXGraphicsDevice> _device;
	ID3D11RasterizerState* _rasterizerState;
};

}}

#endif