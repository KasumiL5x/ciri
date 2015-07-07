#ifndef __ciri_dxgraphicsdevice__
#define __ciri_dxgraphicsdevice__

#include <ciri/Common.hpp>
#include CIRI_INCLUDE_PS(DXGraphicsDevice_ps)
#include "IGraphicsDevice.hpp"

namespace ciri {
	class DXGraphicsDevice : public IGraphicsDevice {
	public:
		DXGraphicsDevice();
		virtual ~DXGraphicsDevice();

		virtual bool create( Window* window );

	private:
		DXGraphicsDevice_ps _platform;
	};
}

#endif /*  __ciri_dxgraphicsdevice__  */