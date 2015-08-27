#ifndef __idemo__
#define __idemo__

#include <string>
#include <ciri/wnd/Window.hpp>
#include <ciri/gfx/IGraphicsDevice.hpp>
#include <ciri/gfx/GraphicsDeviceFactory.hpp>

struct DemoConfig {
	std::string windowTitle;
	unsigned int windowWidth;
	unsigned int windowHeight;
	ciri::GraphicsDeviceFactory::DeviceType deviceType;

	DemoConfig() {
		windowTitle = "ciri";
		windowWidth = 1280;
		windowHeight = 720;
		deviceType = ciri::GraphicsDeviceFactory::OpenGL;
	}
};

class IDemo {
private:
	bool _shouldGtfo;
	ciri::IGraphicsDevice* _graphicsDevice;
	ciri::Window* _window;

public:
	IDemo()
		: _shouldGtfo(false), _graphicsDevice(nullptr), _window(nullptr) {
	}
	virtual ~IDemo() {
	}

	/**
	 * Sets the graphics device.  This should never be called by implementing demos.
	 * @param device Pointer to the graphics device.
	 */
	void setGraphicsDevice( ciri::IGraphicsDevice* device ) {
		_graphicsDevice = device;
	}

	/**
	 * Gets the graphics device.
	 */
	ciri::IGraphicsDevice* graphicsDevice() const {
		return _graphicsDevice;
	}

	/**
	 * Sets the window.  This should never be called by implementing demos.
	 * @param window Pointer to the window.
	 */
	void setWindow( ciri::Window* window ) {
		_window = window;
	}

	/**
	 * Gets the window.
	 */
	ciri::Window* window() const {
		return _window;
	}

	/**
	 * Returns the configuration for a demo automatically retreived before initalization.
	 * @return DemoConfig used to configure resource creation before initialization of the demo.
	 */
	virtual DemoConfig getConfig()=0;

	/**
	 * Called after non-demo resource creation (window, graphics device, and so on).  Initialize the demo here.
	 */
	virtual void onInitialize()=0;

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

	/**
	 * Closes the demo.
	 */
	void gtfo() {
		_shouldGtfo = true;
	}

	/**
	 * Gets if the user requested for the demo to quit.
	 */
	bool hasRequestedToQuit() const {
		return _shouldGtfo;
	}
};

#endif /* __idemo__ */