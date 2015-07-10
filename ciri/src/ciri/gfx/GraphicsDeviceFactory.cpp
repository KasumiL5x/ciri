#include <ciri/gfx/GraphicsDeviceFactory.hpp>
#include <ciri/gfx/GLGraphicsDevice.hpp>
#include <ciri/gfx/DXGraphicsDevice.hpp>

namespace ciri {
	IGraphicsDevice* GraphicsDeviceFactory::create( DeviceType type ) {
		switch( type ) {
			case GraphicsDeviceFactory::OpenGL: {
				return new GLGraphicsDevice();
			}

			case GraphicsDeviceFactory::DirectX: {
				return new DXGraphicsDevice();
			}

			default: {
				return nullptr;
			}
		}
	}
} // ciri