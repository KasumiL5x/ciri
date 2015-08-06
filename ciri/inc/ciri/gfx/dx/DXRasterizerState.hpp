#ifndef __ciri_dxrasterizerstate__
#define __ciri_dxrasterizerstate__

#include "../IRasterizerState.hpp"
#include <d3d11_1.h>

namespace ciri {
	class DXGraphicsDevice;

	class DXRasterizerState : public IRasterizerState {
	public:
		DXRasterizerState( DXGraphicsDevice* device );
		virtual ~DXRasterizerState();

		bool create( const RasterizerDesc& desc );
		virtual void destroy();

		ID3D11RasterizerState* getRasterizerState() const;

	private:
		DXGraphicsDevice* _device;
		ID3D11RasterizerState* _rasterizerState;
	};
} // ciri

#endif /* __ciri_dxrasterizerstate__ */