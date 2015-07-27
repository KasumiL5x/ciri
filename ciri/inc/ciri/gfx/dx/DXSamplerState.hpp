#ifndef __ciri_dxsamplerstate__
#define __ciri_dxsamplerstate__

#include "../ISamplerState.hpp"

namespace ciri {
	class DXSamplerState : public ISamplerState {
	public:
		DXSamplerState();
		virtual ~DXSamplerState();

		bool create(  const SamplerDesc& desc );
		virtual void destroy();
	};
} // ciri

#endif /* __ciri_dxsamplerstate__ */