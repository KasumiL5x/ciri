#ifndef __ciri_graphics_DXBlendState__
#define __ciri_graphics_DXBlendState__

#include <memory>
#include <d3d11.h>
#include <ciri/graphics/IBlendState.hpp>

namespace ciri { namespace graphics {

class DXGraphicsDevice;

class DXBlendState : public IBlendState {
public:
	DXBlendState( const std::shared_ptr<DXGraphicsDevice>& device );
	virtual ~DXBlendState();

	bool create( const BlendDesc& desc );
	virtual void destroy() override;

	const BlendDesc& getDesc() const;
	ID3D11BlendState* getBlendState() const;

private:
	std::shared_ptr<DXGraphicsDevice> _device;
	ID3D11BlendState* _blendState;
	BlendDesc _desc;
};

}}

#endif