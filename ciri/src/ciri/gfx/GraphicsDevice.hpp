#ifndef __ciri_graphics_device__
#define __ciri_graphics_device__

#include <ciri/Common.hpp>
#include CIRI_INCLUDE_GFX_PS(GraphicsDevice_ps)

namespace ciri {
	class Window;

	class GraphicsDevice {
	public:
		GraphicsDevice();
		~GraphicsDevice();

		bool create( Window* window );

	private:
		GraphicsDevice_ps _platform;
	};
} // ciri

#endif /* __ciri_graphics_device__ */