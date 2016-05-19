#ifndef __ciri_screensystem_screen_state__
#define __ciri_screensystem_screen_state__

namespace ciri {
	enum class ScreenState {
		Active,   /**< Updates and draws. */
		Inactive, /**< Only draws. */
		Hidden    /**< Nothing. */
	};
}

#endif /* __ciri_screensystem_screen_state__ */