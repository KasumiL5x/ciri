#include <ciri/gfx/dx/DXDepthStencilState.hpp>
#include <ciri/gfx/dx/DXGraphicsDevice.hpp>
#include <ciri/gfx/dx/CiriToDx.hpp>

namespace ciri {
	DXDepthStencilState::DXDepthStencilState( DXGraphicsDevice* device )
		: IDepthStencilState(), _device(device), _state(nullptr) {
	}

	DXDepthStencilState::~DXDepthStencilState() {
		destroy();
	}

	bool DXDepthStencilState::create( const DepthStencilDesc& desc ) {
		if( _state != nullptr ) {
			return false;
		}

		_desc = desc;

		D3D11_DEPTH_STENCIL_DESC dxDesc;
		ZeroMemory(&dxDesc, sizeof(dxDesc));

		dxDesc.DepthEnable = desc.depthEnable;
		dxDesc.DepthWriteMask = (desc.depthWriteMask) ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
		dxDesc.DepthFunc = ciriToDxFunc(desc.depthFunc);
		
		dxDesc.StencilEnable = desc.stencilEnable;
		dxDesc.StencilReadMask = desc.stencilReadMask;
		dxDesc.StencilWriteMask = desc.stencilWriteMask;

		dxDesc.FrontFace.StencilFailOp = ciriToDxOp(desc.frontStencilFailOp);
		dxDesc.FrontFace.StencilDepthFailOp = ciriToDxOp(desc.frontStencilDepthFailOp);
		dxDesc.FrontFace.StencilPassOp = ciriToDxOp(desc.frontStencilPassOp);
		dxDesc.FrontFace.StencilFunc = ciriToDxFunc(desc.frontStencilFunc);

		if( desc.twoSidedStencil ) {
			dxDesc.BackFace.StencilFailOp = ciriToDxOp(desc.backStencilFailOp);
			dxDesc.BackFace.StencilDepthFailOp = ciriToDxOp(desc.backStencilDepthFailOp);
			dxDesc.BackFace.StencilPassOp = ciriToDxOp(desc.backStencilPassOp);
			dxDesc.BackFace.StencilFunc = ciriToDxFunc(desc.backStencilFunc);
		} else {
			dxDesc.BackFace.StencilFailOp = dxDesc.FrontFace.StencilFailOp;
			dxDesc.BackFace.StencilDepthFailOp = dxDesc.FrontFace.StencilDepthFailOp;
			dxDesc.BackFace.StencilPassOp = dxDesc.FrontFace.StencilPassOp;
			dxDesc.BackFace.StencilFunc = dxDesc.FrontFace.StencilFunc;
		}

		if( FAILED(_device->getDevice()->CreateDepthStencilState(&dxDesc, &_state)) ) {
			return false;
		}

		return true;
	}

	void DXDepthStencilState::destroy() {
		if( _state != nullptr ) {
			_state->Release();
			_state = nullptr;
		}
	}

	ID3D11DepthStencilState* DXDepthStencilState::getState() {
		return _state;
	}

	int DXDepthStencilState::getStencilRef() const {
		return _desc.stencilRef;
	}
} // ciri