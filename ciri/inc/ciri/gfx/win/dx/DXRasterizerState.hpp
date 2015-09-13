#ifndef __ciri_gfx_dxrasterizerstate__
#define __ciri_gfx_dxrasterizerstate__

#include <memory>
#include <d3d11.h>
#include <ciri/gfx/IRasterizerState.hpp>

namespace ciri {
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
} // ciri

#endif /* __ciri_gfx_dxrasterizerstate__ */
