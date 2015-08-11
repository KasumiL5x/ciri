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
		D3D11_TEXTURE_ADDRESS_MODE ciriToDxWrap( SamplerWrap::Mode mode ) const;
		D3D11_COMPARISON_FUNC ciriToDxFunc( SamplerComparison::Mode mode ) const;
		D3D11_FILTER ciriToDxFilter( SamplerFilter::Mode mode, bool comparison ) const;

	private:
		DXGraphicsDevice* _device;
		ID3D11SamplerState* _samplerState;
	};
} // ciri

#endif /* __ciri_dxsamplerstate__ */