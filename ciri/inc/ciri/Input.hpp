#ifndef __ciri_input__
#define __ciri_input__

#include <memory>
#include "input/IInput.hpp"
#include "input/Keyboard.hpp"
#include "input/Mouse.hpp"

namespace ciri {
	/**
	 * Creates a new input.
	 * Note that this function is defined in the linked input library, not in ciri itself.
	 * @returns Pointer to a new input.
	 */
	std::shared_ptr<IInput> createInput();
} // ciri

#endif /* __ciri_input__ */