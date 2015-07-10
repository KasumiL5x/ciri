#include <ciri/wnd/Window.hpp>
#include <ciri/wnd/WindowEvent.hpp>
#include <ciri/gfx/GraphicsDeviceFactory.hpp>

int main() {
	ciri::Window window;
	window.create(1280, 720);

	ciri::IGraphicsDevice* device = ciri::GraphicsDeviceFactory::create(ciri::GraphicsDeviceFactory::DirectX);
	if( !device->create(&window) ) {
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
		device->present();
	}

	device->destroy();
	delete device;
	device = nullptr;

	return 0;
}