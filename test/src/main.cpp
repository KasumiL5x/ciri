#include <ciri/wnd/Window.hpp>
#include <ciri/wnd/WindowEvent.hpp>
#include <ciri/gfx/IGraphicsDevice.hpp>
#include <ciri/gfx/DXGraphicsDevice.hpp>

int main() {
	ciri::Window window;
	window.create(1280, 720);

	ciri::IGraphicsDevice* graphicsDevice = new ciri::DXGraphicsDevice();
	if( !graphicsDevice->create(&window) ) {
		printf("Failed to initialize graphics device.");
		return -1;
	}

	while( window.isOpen() ) {
		ciri::WindowEvent evt;
		while( window.pollEvent(evt) ) {
			if( evt.type == ciri::WindowEvent::Closed ) {
				window.destroy();
			}

			if( evt.type == ciri::WindowEvent::Resized ) {
				//printf("w: %d; h: %d\n", evt.size.width, evt.size.height);
			}

			if( evt.type == ciri::WindowEvent::FocusGained ) {
				//printf("focus gained\n");
			}

			if( evt.type == ciri::WindowEvent::FocusLost ) {
				//printf("focus lost\n");
			}
		}

		// render and flip
		graphicsDevice->present();
	}

	graphicsDevice->destroy();
	delete graphicsDevice;
	graphicsDevice = nullptr;

	return 0;
}