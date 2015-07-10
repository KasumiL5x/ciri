#include "GraphicsDeviceFactory.hpp"
#include "GLGraphicsDevice.hpp"
#include "DXGraphicsDevice.hpp"

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