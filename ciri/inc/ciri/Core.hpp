#ifndef __ciri_core__
#define __ciri_core__

#include "core/ErrorCodes.hpp"
#include "core/File.hpp"
#include "core/ITimer.hpp"
#include "core/Log.hpp"
#include "core/PNG.hpp"
#include "core/StrUtil.hpp"
#include "core/TGA.hpp"

namespace ciri {
	/**
	 * Creates a new timer.
	 * Note that this function is defined in the linked window library, not in ciri itself.
	 * @return Pointer to a new timer.
	 */
	std::shared_ptr<ITimer> createTimer();
} // ciri

#endif /* __ciri_core__ */