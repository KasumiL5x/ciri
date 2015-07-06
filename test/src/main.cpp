#include <ciri/wnd/Window.hpp>
#include <ciri/wnd/WindowEvent.hpp>

int main() {
	ciri::Window window;
	window.create(640, 480);
	while( window.isOpen() ) {
		ciri::WindowEvent evt;
		while( window.pollEvent(evt) ) {
			if( evt.type == ciri::WindowEvent::Closed ) {
				window.destroy();
			}
		}
	}

	return 0;
}