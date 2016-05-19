#ifndef __ciri_screensystem_screen_manager__
#define __ciri_screensystem_screen_manager__

#include <memory>
#include <unordered_map>
#include <string>
#include <list>
#include "ScreenState.hpp"

namespace ciri {
	class Screen;

	class ScreenManager {
	public:
		ScreenManager();
		~ScreenManager();

		/**
		 * Adds a new named Screen.
		 * @param screen Screen instance to add.
		 * @param name   Identifier the Screen will be known by.
		 * @param activate If true, the Screen will be activated, otherwise, it will be hidden.
		 * @returns False if the screen name is already taken or the Screen is null.
		 */
		bool add( const std::shared_ptr<Screen>& screen, const std::string& name, bool activate );

		/**
		 * Removes a Screen by name.
		 * @param name Identifier of the Screen to remove.
		 * @returns False if the name is not found.
		 */
		bool remove( const std::string& name );

		/**
		 * Removes a Screen directly.
		 * @param screen Screen to remove.
		 * @returns False if the Screen is null or not added.
		 */
		bool remove( const std::shared_ptr<Screen>& screen );

		/**
		 * Updates all active Screens.
		 * Screens are ordered by Z-Index before calling update and therefore ordering changes will not take place until the following update.
		 * Updates and draws are done by Z-index starting from smallest to largest.
		 * @param deltaTime   Time since last frame in seconds.
		 * @param elapsedTime Time since program startup in seconds.
		 */
		void update( double deltaTime, double elapsedTime );

		/**
		 * Updates all active Screens at a fixed timestep.
		 * @param deltaTime   Fixed delta time.
		 * @param elapsedTime Time since program startup in seconds.
		 */
		void updateFixed( double deltaTime, double elapsedTime );

		/**
		 * Draws all active Screens.
		 */
		void draw();

		/**
		 * Activates a Screen by name.  Activated Screens are both updated and drawn.
		 * @param name Name of the Screen to activate.
		 * @returns False if the named Screen was not found, or if it was already activated.
		 */
		bool activate( const std::string& name );

		/**
		 * Activates a Screen directly.  Activated Screens are both updated and drawn.
		 * @param screen Screen to activate.
		 * @returns False if the Screen was not found, or if it was already active.
		 */
		bool activate( const std::shared_ptr<Screen>& screen );

		/**
		 * Deactivates a Screen by name.  Deactivated Screens are drawn but not updated.
		 * @param name Name of the Screen to deactivate.
		 * @returns False if the named Screen was not found, or if it was already deactivated.
		 */
		bool deactivate( const std::string& name );

		/**
		 * Deactivates a Screen directly.  Deactivated Screens are drawn but not updated.
		 * @param screen Screen to deactivate.
		 * @returns False if the Screen was not found, or if it was already deactivated.
		 */
		bool deactivate( const std::shared_ptr<Screen>& screen );

		/**
		 * Hides a Screen by name.  Hidden screens are not updated or drawn.
		 * @param name Name of the Screen to hide.
		 * @returns False if the named Screen was not found, or if it was already hidden.
		 */
		bool hide( const std::string& name );
		/**
		 * Hides a Screen directly.  Hidden screens are not updated or drawn.
		 * @param screen Screen to hide.
		 * @returns False if the Screen was not found, or if it was already hidden.
		 */
		bool hide( const std::shared_ptr<Screen>& screen );

	private:
		std::unordered_map<std::string, std::shared_ptr<Screen>> _screenNames; /**< Screen names => Screens */
		std::unordered_map<std::shared_ptr<Screen>, ScreenState> _states;  /**< Screens => ScreenStates */
		std::vector<std::shared_ptr<Screen>> _orderedScreens;              /**< Screens in z-index order. */
	};
}

#endif /* __ciri_screensystem_screen_manager__ */