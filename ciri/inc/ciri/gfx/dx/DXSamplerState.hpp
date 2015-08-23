#ifndef __ciri_dxsamplerstate__
#define __ciri_dxsamplerstate__

#include <d3d11_1.h>
#include "../ISamplerState.hpp"

namespace ciri {
	class DXGraphicsDevice;

	class DXSamplerState : public ISamplerState {
	public:
		DXSamplerState( DXGraphicsDevice* device );
		virtual ~DXSamplerState();

		bool create(  const SamplerDesc& desc );
		virtual void destroy();

		ID3D11SamplerState* getSamplerState() const;

	private:
		DXGraphicsDevice* _device;
		ID3D11SamplerState* _samplerState;
	};
} // ciri

#endif /* __ciri_dxsamplerstate__ */