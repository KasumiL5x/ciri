#include <ciri/gfx/dx/DXSamplerState.hpp>

namespace ciri {
	DXSamplerState::DXSamplerState()
		: ISamplerState() {
	}

	DXSamplerState::~DXSamplerState() {
		destroy();
	}

	bool DXSamplerState::create( const SamplerDesc& desc ) {
		return false;
	}

	void DXSamplerState::destroy() {
	}
} // ciri