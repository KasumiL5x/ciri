#ifndef __ciri_core_Input__
#define __ciri_core_Input__

#include <ciri/core/input/IInput.hpp>
#include <Windows.h>

namespace ciri { namespace core {

struct KeyboardState {
	const static int KEY_ARRAY_SIZE = 256;
	static unsigned char _keyBuffer[KEY_ARRAY_SIZE]; // shared between all states; used by win32 code
	bool states[KEY_ARRAY_SIZE]; // per state

	KeyboardState();
	KeyboardState( const KeyboardState& rhs );
	KeyboardState& operator=( const KeyboardState& rhs );
};

struct MouseState {
	bool states[(int)MouseButton::Max];
	int x;
	int y;

	MouseState();
	MouseState( const MouseState& rhs );
	MouseState& operator=( const MouseState& rhs );
};

class Input : public IInput {
public:
	Input();
	virtual ~Input();

	virtual bool create( std::shared_ptr<IWindow> window ) override;
	virtual bool poll() override;
	virtual bool update() override;
	virtual bool isKeyDown( Key key ) const override;
	virtual bool isKeyUp( Key key ) const override;
	virtual bool wasKeyDown( Key key ) const override;
	virtual bool wasKeyUp( Key key ) const override;
	virtual bool isMouseButtonDown( MouseButton button ) const override;
	virtual bool isMouseButtonUp( MouseButton button ) const override;
	virtual bool wasMouseButtonDown( MouseButton button ) const override;
	virtual bool wasMouseButtonUp( MouseButton button ) const override;
	virtual int mouseX() const override;
	virtual int mouseY() const override;
	virtual int lastMouseX() const override;
	virtual int lastMouseY() const override;

private:
	KeyboardState _currKeyState;
	KeyboardState _prevKeyState;
	MouseState _currMouseState;
	MouseState _prevMouseState;

	std::shared_ptr<IWindow> _window;
};

}}

#endif