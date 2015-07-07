#include <ciri/wnd/Window.hpp>
#include <ciri/wnd/WindowEvent.hpp>

int main() {
	ciri::Window window;
	window.create(1280, 720);
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
	}

	return 0;
}