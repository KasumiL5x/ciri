#ifndef __ciri_screensystem_screen__
#define __ciri_screensystem_screen__

#include "ScreenState.hpp"

namespace ciri {
	class Screen {
	public:
		Screen() {
		}

		virtual ~Screen() {
		}

		/**
		 * Called when the Screen is added to the ScreenManager.
		 */
		virtual void onAdd()=0;

		/**
		 * Called just before the Screen is removed from the ScreenManager.
		 */
		virtual void onRemove()=0;

		/**
		 * Update their logic here.
		 * @param deltaTime   Time since last frame in seconds.
		 * @param elapsedTime Time since program startup in seconds.
		 */
		virtual void onUpdate( double deltaTime, double elapsedTime )=0;

		/**
		 * Update logic that depends on a fixed timestep here.
		 * @param deltaTime   Fixed delta time.
		 * @param elapsedTime Time since program startup in seconds.
		 */
		virtual void onFixedUpdate( double deltaTime, double elapsedTime )=0;

		/**
		 * Draw here.
		 */
		virtual void onDraw()=0;

		/**
		 * Return the z-index used for drawing order.  Higher indices are drawn in front.
		 */
		virtual int zIndex() const=0;
	};
}

#endif /* __ciri_screensystem_screen__ */