#ifndef __ciri_gfx_dxblendstate__
#define __ciri_gfx_dxblendstate__

#include <memory>
#include <d3d11.h>
#include <ciri/gfx/IBlendState.hpp>

namespace ciri {
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
} // ciri

#endif /* __ciri_gfx_dxblendstate__ */