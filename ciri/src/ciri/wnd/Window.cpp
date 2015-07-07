#include <ciri/wnd/Window.hpp>

namespace ciri {
	Window::Window() {
	}

	Window::~Window() {
	}

	bool Window::create( int width, int height ) {
		return _platform.create(width, height);
	}

	bool Window::isOpen() const {
		return _platform.isOpen();
	}

	bool Window::pollEvent( WindowEvent& evt ) {
		return _platform.pollEvent(evt);
	}

	void Window::destroy() {
		_platform.destroy();
	}

	cc::Vec2ui Window::getSize() const {
		return _platform.getSize();
	}

	void* Window::getHandle() const {
		return _platform.getHandle();
	}

} // ciri