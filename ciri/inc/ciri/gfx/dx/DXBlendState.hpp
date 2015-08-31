#ifndef __ciri_dxblendstate__
#define __ciri_dxblendstate__

#include <d3d11.h>
#include "../IBlendState.hpp"

namespace ciri {
	class DXGraphicsDevice;

	class DXBlendState : public IBlendState {
	public:
		DXBlendState( DXGraphicsDevice* device );
		virtual ~DXBlendState();

		bool create( const BlendDesc& desc );
		virtual void destroy() override;

		ID3D11BlendState* getBlendState() const;

	private:
		DXGraphicsDevice* _device;
		ID3D11BlendState* _blendState;
	};
} // ciri

#endif /* __ciri_dxblendstate__ */
