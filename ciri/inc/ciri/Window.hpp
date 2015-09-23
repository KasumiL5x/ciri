#ifndef __ciri_window__
#define __ciri_window__

#include <memory>
#include "wnd/WindowEvent.hpp"
#include "wnd/IWindow.hpp"

namespace ciri {
	/**
	 * Creates a new window.
	 * Note that this function is defined in the linked window library, not in ciri itself.
	 * @return Pointer to a new window.
	 */
	std::shared_ptr<IWindow> createWindow();
} // ciri

#endif /* __ciri_window__ */