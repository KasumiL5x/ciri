#ifndef __ciri_graphics_DXSamplerState__
#define __ciri_graphics_DXSamplerState__

#include <memory>
#include <d3d11.h>
#include <ciri/graphics/ISamplerState.hpp>

namespace ciri { namespace graphics {

class DXGraphicsDevice;

class DXSamplerState : public ISamplerState {
public:
	DXSamplerState( const std::shared_ptr<DXGraphicsDevice>& device );
	virtual ~DXSamplerState();

	bool create(  const SamplerDesc& desc );
	virtual void destroy() override;

	ID3D11SamplerState* getSamplerState() const;

private:
	std::shared_ptr<DXGraphicsDevice> _device;
	ID3D11SamplerState* _samplerState;
};

}}

#endif