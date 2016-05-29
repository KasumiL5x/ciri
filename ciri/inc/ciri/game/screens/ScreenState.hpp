#ifndef __ciri_game_ScreenState__
#define __ciri_game_ScreenState__

namespace ciri {

enum class ScreenState {
	Active,   /**< Updates and draws. */
	Inactive, /**< Only draws. */
	Hidden    /**< Nothing. */
};

}

#endif