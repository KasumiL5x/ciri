#include <crtdbg.h>
#include <memory>
#include <ciri/Window.hpp>
#include <ciri/Graphics.hpp>
#include <ciri/Input.hpp>
#include <ciri/Core.hpp>
#include "demos/dynvb/DynamicVertexBufferDemo.hpp"
#include "demos/terrain/TerrainDemo.hpp"
#include "demos/sprites/SpritesDemo.hpp"

enum class Demo {
	Dynvb,
	Terrain,
	Sprites
};

std::unique_ptr<IDemo> createDemo( Demo type ) {
	switch( type ) {
		case Demo::Dynvb: {
			return std::unique_ptr<IDemo>(new DynamicVertexBufferDemo());
		}

		case Demo::Terrain:{ 
			return std::unique_ptr<IDemo>(new TerrainDemo());
		}

		case Demo::Sprites: {
			return std::unique_ptr<IDemo>(new SpritesDemo());
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

	// create the demo and get its configuration
	std::unique_ptr<IDemo> demo = createDemo(Demo::Sprites);
	const DemoConfig config = demo->getConfig();

	// create window
	std::shared_ptr<ciri::IWindow> window = ciri::createWindow();
	if( !window->create(config.windowWidth, config.windowHeight) ) {
		return -1;
	}

	// set window title
	window->setWindowText(config.windowTitle.c_str());

	// create and initialize input
	std::shared_ptr<ciri::IInput> input = ciri::createInput();
	if( !input->create(window) ) {
		return -1;
	}

	// create and initialize the graphics device
	std::shared_ptr<ciri::IGraphicsDevice> graphicsDevice = ciri::createGraphicsDevice();
	if( !graphicsDevice->create(window) ) {
		return -1;
	}

	// set the demo's resources
	demo->setWindow(window);
	demo->setInput(input);
	demo->setGraphicsDevice(graphicsDevice);

	// initialize and load the demo's content
	demo->onInitialize();
	demo->onLoadContent();

	// timer for delta and elapsed timing
	double elapsedTime = 0.0;
	double lastTime = 0.0;
	std::shared_ptr<ciri::ITimer> timer = ciri::createTimer();
	timer->start();

	// main loop
	const double MS_PER_UPDATE = 1.0f / 60.0f;
	double lag = 0.0;
	bool running = true;
	while( running && !demo->hasRequestedToQuit() ) {
		// event processing
		ciri::WindowEvent evt;
		while( window->pollEvent(evt) ) {
			demo->onEvent(evt);

			if( ciri::WindowEvent::Closed == evt.type ) {
				running = false;
				break;
			}
		}

		// delta and elapsed time
		const double currTime = timer->getElapsedMicrosecs();
		const double deltaTime = (currTime - lastTime) * 0.000001;
		lastTime = currTime;
		elapsedTime += deltaTime;
		lag += deltaTime;

		// poll input
		input->poll();

		// standard update run every frame
		demo->onUpdate(deltaTime, elapsedTime);

		// catch-up update
		while( lag >= MS_PER_UPDATE ) {
			demo->onFixedUpdate(MS_PER_UPDATE, elapsedTime);
			lag -= MS_PER_UPDATE;
		}

		// update input
		input->update();

		// draw
		demo->onDraw();
	}

	// clean demo
	demo->onUnloadContent();
	demo.reset();

	// clean graphic device
	graphicsDevice->destroy();
	graphicsDevice.reset();

	// clean window
	window->close();
	window.reset();

	return 0;
}