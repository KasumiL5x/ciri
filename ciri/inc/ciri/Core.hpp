#ifndef __ciri_core_Core__
#define __ciri_core_Core__

#include <memory>
#include <ciri/core/ErrorCodes.hpp>
#include <ciri/core/File.hpp>
#include <ciri/core/ITimer.hpp>
#include <ciri/core/Leb128.hpp>
#include <ciri/core/Log.hpp>
#include <ciri/core/PNG.hpp>
#include <ciri/core/StrUtil.hpp>
#include <ciri/core/TGA.hpp>
#include <ciri/core/window/IWindow.hpp>
#include <ciri/core/window/WindowEvent.hpp>
#include <ciri/core/input/IInput.hpp>
#include <ciri/core/input/Keyboard.hpp>
#include <ciri/core/input/Mouse.hpp>

namespace ciri {

/**
 * Creates a new timer.
 * Note that this function is implemented in platform-specific code.
 * @returns Pointer to a new timer.
 */
std::shared_ptr<ITimer> createTimer();

/**
 * Creates a new window.
 * Note that this function is implemented in platform-specific code.
 * @returns Pointer to a new window.
 */
std::shared_ptr<IWindow> createWindow();

/**
 * Creates a new input.
 * Note that this function is implemented in platform-specific code.
 * @returns Pointer to a new input.
 */
std::shared_ptr<IInput> createInput();

}

#endif