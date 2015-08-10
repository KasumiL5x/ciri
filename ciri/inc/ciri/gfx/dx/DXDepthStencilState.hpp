#ifndef __ciri_dxdepthstencilstate__
#define __ciri_dxdepthstencilstate__

#include <d3d11.h>
#include "../IDepthStencilState.hpp"

namespace ciri {
	class DXGraphicsDevice;

	class DXDepthStencilState : public IDepthStencilState {
	public:
		DXDepthStencilState( DXGraphicsDevice* device );
		virtual ~DXDepthStencilState();

		bool create( const DepthStencilDesc& desc );
		virtual void destroy();

		ID3D11DepthStencilState* getState();
		int getStencilRef() const;

	private:
		D3D11_COMPARISON_FUNC ciriToDxFunc( CompareFunction::Function func ) const;
		D3D11_STENCIL_OP ciriToDxOp( StencilOperation::Operation op ) const;

	private:
		DXGraphicsDevice* _device;
		ID3D11DepthStencilState* _state;
		DepthStencilDesc _desc;
	};
} // ciri

#endif /* __ciri_dxdepthstencilstate__ */