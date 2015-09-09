#include <ciri/core/Window.hpp>
#include <ciri/core/Timer.hpp>
#include "demos/dynvb/DynamicVertexBufferDemo.hpp"
#include "demos/terrain/TerrainDemo.hpp"
#ifdef _DEBUG
#include <crtdbg.h>
#endif

enum class DemoType {
	DynamicVertexBuffer,
	Terrain
};

IDemo* createDemo( DemoType type ) {
	switch( type ) {
		case DemoType::DynamicVertexBuffer: {
			return new DynamicVertexBufferDemo();
		}

		case DemoType::Terrain: {
			return new TerrainDemo();
		}

		default: {
			return nullptr;
		}
	}
}

int main() {
	// enable memory leak checking
#ifdef _DEBUG
	int debugFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	debugFlag|= _CRTDBG_LEAK_CHECK_DF;
	debugFlag |= _CRTDBG_CHECK_ALWAYS_DF;
	_CrtSetDbgFlag(debugFlag);
#endif

	// create the demo and get its configuration settings
	IDemo* demo = createDemo(DemoType::Terrain);
	const DemoConfig config = demo->getConfig();

	// create window
	ciri::Window window;
	if( !window.create(config.windowWidth, config.windowHeight) ) {
		return -1;
	}

	// set window title
	window.setTitle(config.windowTitle.c_str());

	// create and initialize a graphics device
	ciri::IGraphicsDevice* graphicsDevice = ciri::createGraphicsDevice();
	if( !graphicsDevice->create(&window) ) {
		return -1;
	}

	// set the demo's window and graphics device
	demo->setWindow(&window);
	demo->setGraphicsDevice(graphicsDevice);

	// initialize the demo and load its content
	demo->onInitialize();
	demo->onLoadContent();

	// timer for delta and elapsed time measurement
	double elapsedTime = 0.0;
	double lastTime = 0.0;
	ciri::Timer timer;
	timer.start();

	// main loop
	bool running = true;
	while( running && !demo->hasRequestedToQuit() ) {
		// event processing
		ciri::WindowEvent evt;
		while( window.pollEvent(evt) ) {
			demo->onEvent(evt);

			if( ciri::WindowEvent::Closed == evt.type ) {
				running = false;
				break;
			}
		}

		// delta and elapsed time
		const double currTime = timer.getElapsedMicroseconds();
		const double deltaTime = (currTime - lastTime) * 0.000001;
		lastTime = currTime;
		elapsedTime += deltaTime;

		// update and draw
		demo->onUpdate(deltaTime, elapsedTime);
		demo->onDraw();
	}

	// clean demo
	demo->onUnloadContent();
	delete demo;

	// clean graphics device
	graphicsDevice->destroy();
	delete graphicsDevice;

	// clean window
	window.destroy();

	return 0;
}