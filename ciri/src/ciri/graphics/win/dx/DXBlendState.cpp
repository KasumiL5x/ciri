#include <ciri/graphics/win/dx/DXBlendState.hpp>
#include <ciri/graphics/win/dx/DXGraphicsDevice.hpp>
#include <ciri/graphics/win/dx/CiriToDx.hpp>

using namespace ciri;

DXBlendState::DXBlendState( const std::shared_ptr<DXGraphicsDevice>& device )
	: IBlendState(), _device(device), _blendState(nullptr) {
}

DXBlendState::~DXBlendState() {
	destroy();
}

bool DXBlendState::create( const BlendDesc& desc ) {
	D3D11_BLEND_DESC dxDesc;
	ZeroMemory(&dxDesc, sizeof(dxDesc));

	const bool blendEnable = desc.blendingEnabled();
	const UINT8 writeMask = ciriToDxBlendColorMask(desc.colorMask);
	for( int i = 0; i < 8; ++i ) { // MS hardcoded to 8, unlikely to change
		dxDesc.RenderTarget[i].BlendEnable = blendEnable;
		dxDesc.RenderTarget[i].SrcBlend = ciriToDxBlendMode(desc.srcColorBlend, false);
		dxDesc.RenderTarget[i].DestBlend = ciriToDxBlendMode(desc.dstColorBlend, false);
		dxDesc.RenderTarget[i].BlendOp = ciriToDxBlendFunction(desc.colorFunc);
		dxDesc.RenderTarget[i].SrcBlendAlpha = ciriToDxBlendMode(desc.srcAlphaBlend, true);
		dxDesc.RenderTarget[i].DestBlendAlpha = ciriToDxBlendMode(desc.dstAlphaBlend, true);
		dxDesc.RenderTarget[i].BlendOpAlpha = ciriToDxBlendFunction(desc.alphaFunc);
		dxDesc.RenderTarget[i].RenderTargetWriteMask = ciriToDxBlendColorMask(desc.colorMask);
	}

	dxDesc.AlphaToCoverageEnable = false; // todo
	dxDesc.IndependentBlendEnable = false; // todo (if GL side supports eventually)

	if( FAILED(_device->getDevice()->CreateBlendState(&dxDesc, &_blendState)) ) {
		return false;
	}

	_desc = desc;

	return true;
}

void DXBlendState::destroy() {
	if( _blendState != nullptr ) {
		_blendState->Release();
		_blendState = nullptr;
	}
}

const BlendDesc& DXBlendState::getDesc() const {
	return _desc;
}

ID3D11BlendState* DXBlendState::getBlendState() const {
	return _blendState;
}