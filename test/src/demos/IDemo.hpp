#ifndef __idemo__
#define __idemo__

#include <ciri/wnd/Window.hpp>
#include <ciri/gfx/IGraphicsDevice.hpp>
#include <ciri/gfx/GraphicsDeviceFactory.hpp>

struct DemoConfig {
	unsigned int windowWidth;
	unsigned int windowHeight;
	ciri::GraphicsDeviceFactory::DeviceType deviceType;

	DemoConfig() {
		windowWidth = 1280;
		windowHeight = 720;
		deviceType = ciri::GraphicsDeviceFactory::OpenGL;
	}
};

class IDemo {
public:
	IDemo() {
	}
	virtual ~IDemo() {
	}

	/**
	 * Returns the configuration for a demo automatically retreived before initalization.
	 * @return DemoConfig used to configure resource creation before initialization of the demo.
	 */
	virtual DemoConfig getConfig()=0;

	/**
	 * Called after non-demo resource creation (window, graphics device, and so on).  Initialize the demo here.
	 * @param[in] window The window used for the demo.
	 * @param[in] graphicsDevice The IGraphicsDevice used for rendering in the demo.
	 */
	virtual void onInitialize( ciri::Window* window, ciri::IGraphicsDevice* graphicsDevice )=0;

	/**
	 * Called after onInitialize.  Load resources here.
	 */
	virtual void onLoadContent()=0;

	/**
	 * Called when the window receives an event.  Respond to specific window events here.
	 * @param[in] evt The event code and data.
	 */
	virtual void onEvent( ciri::WindowEvent evt )=0;
	
	/**
	 * Called every loop before rendering.  Update the demo's logic here.
	 * @param[in] deltaTime Time elapsed since the last update.
	 * @param[in] elapsedTime Time elapsed since the main loop started.
	 */
	virtual void onUpdate( double deltaTime, double elapsedTime )=0;

	/**
	 * Called every loop after all updating.  Draw the demo here.
	 */
	virtual void onDraw()=0;

	/**
	 * Called upon destruction of the demo.  Unload resources here.
	 */
	virtual void onUnloadContent()=0;
};

#endif /* __idemo__ */