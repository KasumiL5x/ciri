#ifndef __ciri_graphicsdevicefactory__
#define __ciri_graphicsdevicefactory__

#include "IGraphicsDevice.hpp"

namespace ciri {
	class GraphicsDeviceFactory {
	private:
		GraphicsDeviceFactory() {
		}

	public:
		enum DeviceType {
			OpenGL,
			DirectX,
			MAX
		};

	public:
		static IGraphicsDevice* create( DeviceType type );
	};
} // ciri

#endif /* __ciri_graphicsdevicefactory__ */