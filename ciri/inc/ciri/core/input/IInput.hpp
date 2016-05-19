#ifndef __ciri_core__IInput__
#define __ciri_core__IInput__

#include <memory>
#include "Keyboard.hpp"
#include "Mouse.hpp"

namespace ciri { namespace core {

class IWindow;

class IInput {
public:
	IInput() {
	}

	virtual ~IInput() {
	}

	/**
		* Creates the input.
		* @param window Window to use for input.
		* @returns True on success; false otherwise.
		*/
	virtual bool create( std::shared_ptr<IWindow> window )=0;

	/**
		* Poll the current status of input devices.
		* @returns True on success; false otherwise.
		*/
	virtual bool poll()=0;

	/**
		* Updates the previous states with current states.
		*/
	virtual bool update()=0;

	/**
		* Gets if a key is currently pressed.
		* 
		* @param key Key to check.
		* @return True if key is currently pressed; false otherwise.
		*/
	virtual bool isKeyDown( Key key ) const=0;

	/**
		* Gets if a key is currently unpressed.
		* 
		* @param key Key to check.
		* @return True if the key is currently unpressed; false otherwise.
		*/
	virtual bool isKeyUp( Key key ) const=0;

	/**
		* Gets if a key was previously pressed.
		* 
		* @param key Key to check.
		* @return True if the key was previously pressed; false otherwise.
		*/
	virtual bool wasKeyDown( Key key ) const=0;

	/**
		* Gets if a key was previously unpressed.
		* 
		* @param key Key to check.
		* @return True if the key was previously unpressed; false otherwise.
		*/
	virtual bool wasKeyUp( Key key ) const=0;

	/**
		* Gets if a mouse button is currently pressed.
		* 
		* @param button Button to check.
		* @return True if the button is currently pressed; false otherwise.
		*/
	virtual bool isMouseButtonDown( MouseButton button ) const=0;

	/**
		* Gets if a mouse button is currently unpressed.
		* 
		* @param button Button to check.
		* @return True if the button is currently unpressed; false otherwise.
		*/
	virtual bool isMouseButtonUp( MouseButton button ) const=0;

	/**
		* Gets if a mouse button was previously pressed.
		* 
		* @param button Button to check.
		* @return True if the button was previously pressed; false otherwise.
		*/
	virtual bool wasMouseButtonDown( MouseButton button ) const=0;

	/**
		* Gets if a mouse button was previously unpressed.
		* 
		* @param button Button to check.
		* @return True if the button was previously unpressed; false otherwise.
		*/
	virtual bool wasMouseButtonUp( MouseButton button ) const=0;

	/**
		* Gets the current X position of the mouse.
		* @return X position of the mouse.
		*/
	virtual int mouseX() const=0;

	/**
		* Gets the current Y position of the mouse.
		* @return Y position of the mouse.
		*/
	virtual int mouseY() const=0;

	/**
		* Gets the previous X position of the mouse.
		* @return Previous X position of the mouse.
		*/
	virtual int lastMouseX() const=0;

	/**
		* Gets the previous Y position of the mouse.
		* @return Previous Y position of the mouse.
		*/
	virtual int lastMouseY() const=0;
};

}}

#endif