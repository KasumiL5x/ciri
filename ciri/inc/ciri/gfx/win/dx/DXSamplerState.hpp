#ifndef __ciri_gfx_dxsamplerstate__
#define __ciri_gfx_dxsamplerstate__

#include <d3d11.h>
#include <ciri/gfx/ISamplerState.hpp>

namespace ciri {
	class DXGraphicsDevice;

	class DXSamplerState : public ISamplerState {
	public:
		DXSamplerState( DXGraphicsDevice* device );
		virtual ~DXSamplerState();

		bool create(  const SamplerDesc& desc );
		virtual void destroy() override;

		ID3D11SamplerState* getSamplerState() const;

	private:
		DXGraphicsDevice* _device;
		ID3D11SamplerState* _samplerState;
	};
} // ciri

#endif /* __ciri_gfx_dxsamplerstate__ */
