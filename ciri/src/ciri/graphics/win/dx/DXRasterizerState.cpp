#include <ciri/graphics/win/dx/DXRasterizerState.hpp>
#include <ciri/graphics/win/dx/DXGraphicsDevice.hpp>

using namespace ciri::graphics;

DXRasterizerState::DXRasterizerState( const std::shared_ptr<DXGraphicsDevice>& device )
	: IRasterizerState(), _device(device), _rasterizerState(nullptr) {
}

DXRasterizerState::~DXRasterizerState() {
	destroy();
}

bool DXRasterizerState::create( const RasterizerDesc& desc ) {
	D3D11_RASTERIZER_DESC dxDesc;
	ZeroMemory(&dxDesc, sizeof(dxDesc));

	// cull mode
	switch( desc.cullMode ) {
		case CullMode::None: {
			dxDesc.CullMode = D3D11_CULL_NONE;
			break;
		}
		case CullMode::Clockwise: {
			dxDesc.CullMode = D3D11_CULL_FRONT;
			break;
		}
		case CullMode::CounterClockwise: {
			dxDesc.CullMode = D3D11_CULL_BACK;
			break;
		}
	}
	// fill mode
	if( FillMode::Solid == desc.fillMode ) {
		dxDesc.FillMode = D3D11_FILL_SOLID;
	} else {
		dxDesc.FillMode = D3D11_FILL_WIREFRAME;
	}
	// scissor test
	dxDesc.ScissorEnable = desc.scissorTestEnable;
	// msaa
	dxDesc.MultisampleEnable = desc.msaa;
	// depth bias and slope scaled depth bias
	dxDesc.DepthBias = static_cast<int>(desc.depthBias);
	dxDesc.SlopeScaledDepthBias = desc.slopeScaleDepthBias;
	// depth clip
	dxDesc.DepthClipEnable = desc.depthClipEnable;
	// legacy stuff
	dxDesc.DepthBiasClamp = 0.0f;
	// ignore these
	dxDesc.FrontCounterClockwise = false;
	dxDesc.AntialiasedLineEnable = false;

	if( FAILED(_device->getDevice()->CreateRasterizerState(&dxDesc, &_rasterizerState)) ) {
		destroy();
		return false;
	}

	return true;
}

void DXRasterizerState::destroy() {
	if( _rasterizerState != nullptr ) {
		_rasterizerState->Release();
		_rasterizerState = nullptr;
	}
}

ID3D11RasterizerState* DXRasterizerState::getRasterizerState() const {
	return _rasterizerState;
}