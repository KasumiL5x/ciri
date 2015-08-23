#include <ciri/gfx/dx/DXSamplerState.hpp>
#include <ciri/gfx/dx/DXGraphicsDevice.hpp>
#include <ciri/gfx/dx/CiriToDx.hpp>
#include <cc/Common.hpp>

namespace ciri {
	DXSamplerState::DXSamplerState( DXGraphicsDevice* device )
		: ISamplerState(), _device(device), _samplerState(nullptr) {
	}

	DXSamplerState::~DXSamplerState() {
		destroy();
	}

	bool DXSamplerState::create( const SamplerDesc& desc ) {
		if( _samplerState != nullptr ) {
			return false;
		}

		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));

		samplerDesc.AddressU = ciriToDxWrap(desc.wrapU);
		samplerDesc.AddressV = ciriToDxWrap(desc.wrapV);
		samplerDesc.AddressW = ciriToDxWrap(desc.wrapW);

		samplerDesc.BorderColor[0] = desc.borderColor[0];
		samplerDesc.BorderColor[1] = desc.borderColor[1];
		samplerDesc.BorderColor[2] = desc.borderColor[2];
		samplerDesc.BorderColor[3] = desc.borderColor[3];

		samplerDesc.ComparisonFunc = ciriToDxFunc(desc.comparisonFunc);

		samplerDesc.Filter = ciriToDxFilter(desc.filter, desc.comparisonFunc != CompareFunction::Never);
		const int anisotropy = (SamplerFilter::Anisotropic == desc.filter) ? cc::math::clamp(static_cast<int>(desc.maxAnisotropy), 1, 16) : 1;
		samplerDesc.MaxAnisotropy = anisotropy;

		samplerDesc.MaxLOD = desc.useMipmaps ? desc.maxLod : 0.0f;
		samplerDesc.MinLOD = desc.useMipmaps ? desc.minLod : 0.0f;
		samplerDesc.MipLODBias = desc.lodBias;

		if( FAILED(_device->getDevice()->CreateSamplerState(&samplerDesc, &_samplerState)) ) {
			destroy();
			return false;
		}

		return true;
	}

	void DXSamplerState::destroy() {
		if( _samplerState != nullptr ) {
			_samplerState->Release();
			_samplerState = nullptr;
		}
	}

	ID3D11SamplerState* DXSamplerState::getSamplerState() const {
		return _samplerState;
	}
} // ciri