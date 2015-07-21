#include <ciri/input/Input.hpp>
#include <ciri/wnd/Window.hpp>

namespace ciri {
	unsigned char KeyboardState::_keyBuffer[KEY_ARRAY_SIZE];

	KeyboardState::KeyboardState() {
	}

	KeyboardState::KeyboardState( const KeyboardState& rhs ) {
		memcpy(this->states, rhs.states, sizeof(bool) * KeyboardState::KEY_ARRAY_SIZE);
	}

	KeyboardState& KeyboardState::operator=( const KeyboardState& rhs ) {
		memcpy(this->states, rhs.states, sizeof(bool) * KeyboardState::KEY_ARRAY_SIZE);
		return *this;
	}

	bool KeyboardState::isKeyDown( Keyboard::Key key ) const {
		return states[Input::ciriKeyToWinKey(key)];
	}

	bool KeyboardState::isKeyUp( Keyboard::Key key ) const {
		return !states[Input::ciriKeyToWinKey(key)];
	}

	MouseState::MouseState()
		: x(0), y(0) {
	}

	MouseState::MouseState( const MouseState& rhs ) {
		memcpy(this->states, rhs.states, sizeof(bool) * MouseButton::Max);
		this->x = rhs.x;
		this->y = rhs.y;
	}

	MouseState& MouseState::operator=( const MouseState& rhs ) {
		memcpy(this->states, rhs.states, sizeof(bool) * MouseButton::Max);
		this->x = rhs.x;
		this->y = rhs.y;
		return *this;
	}

	bool MouseState::isButtonDown( MouseButton::Button button ) const {
		return states[button];
	}

	bool MouseState::isButtonUp( MouseButton::Button button ) const {
		return !states[button];
	}

	void Input::getKeyboardState( KeyboardState* outState ) {
		if( nullptr == outState ) {
			return;
		}

		GetKeyboardState(KeyboardState::_keyBuffer);
		for( int i = 0; i < KeyboardState::KEY_ARRAY_SIZE; ++i ) {
			outState->states[i] = (KeyboardState::_keyBuffer[i] & 0x80) ? true : false;
		}
	}

	void Input::getMouseState( MouseState* outState, Window* window ) {
		if( nullptr == outState || nullptr == window ) {
			return;
		}

		outState->states[MouseButton::Left] = (GetAsyncKeyState(VK_LBUTTON) & 0x80) ? true : false;
		outState->states[MouseButton::Middle] = (GetAsyncKeyState(VK_MBUTTON) & 0x80) ? true : false;
		outState->states[MouseButton::Right] = (GetAsyncKeyState(VK_RBUTTON) & 0x80) ? true : false;
		outState->states[MouseButton::X1] = (GetAsyncKeyState(VK_XBUTTON1) & 0x80) ? true : false;
		outState->states[MouseButton::X2] = (GetAsyncKeyState(VK_XBUTTON2) & 0x80) ? true : false;

		POINT pos;
		GetCursorPos(&pos);
		ScreenToClient(window->getHandle(), &pos);
		outState->x = static_cast<int>(pos.x);
		outState->y = static_cast<int>(pos.y);
	}

	int Input::ciriKeyToWinKey( Keyboard::Key key ) {
		#define RETURN_KEY(Ciri, Win) case Ciri: { return Win; }
		switch( key ) {
			RETURN_KEY(Keyboard::Key::A, 'A')
			RETURN_KEY(Keyboard::Key::B, 'B')
			RETURN_KEY(Keyboard::Key::C, 'C')
			RETURN_KEY(Keyboard::Key::D, 'D')
			RETURN_KEY(Keyboard::Key::E, 'E')
			RETURN_KEY(Keyboard::Key::F, 'F')
			RETURN_KEY(Keyboard::Key::G, 'G')
			RETURN_KEY(Keyboard::Key::H, 'H')
			RETURN_KEY(Keyboard::Key::I, 'I')
			RETURN_KEY(Keyboard::Key::J, 'J')
			RETURN_KEY(Keyboard::Key::K, 'K')
			RETURN_KEY(Keyboard::Key::L, 'L')
			RETURN_KEY(Keyboard::Key::M, 'M')
			RETURN_KEY(Keyboard::Key::N, 'N')
			RETURN_KEY(Keyboard::Key::O, 'O')
			RETURN_KEY(Keyboard::Key::P, 'P')
			RETURN_KEY(Keyboard::Key::Q, 'Q')
			RETURN_KEY(Keyboard::Key::R, 'R')
			RETURN_KEY(Keyboard::Key::S, 'S')
			RETURN_KEY(Keyboard::Key::T, 'T')
			RETURN_KEY(Keyboard::Key::U, 'U')
			RETURN_KEY(Keyboard::Key::V, 'V')
			RETURN_KEY(Keyboard::Key::W, 'W')
			RETURN_KEY(Keyboard::Key::X, 'X')
			RETURN_KEY(Keyboard::Key::Y, 'Y')
			RETURN_KEY(Keyboard::Key::Z, 'Z')
			RETURN_KEY(Keyboard::Key::Num0, '0')
			RETURN_KEY(Keyboard::Key::Num1, '1')
			RETURN_KEY(Keyboard::Key::Num2, '2')
			RETURN_KEY(Keyboard::Key::Num3, '3')
			RETURN_KEY(Keyboard::Key::Num4, '4')
			RETURN_KEY(Keyboard::Key::Num5, '5')
			RETURN_KEY(Keyboard::Key::Num6, '6')
			RETURN_KEY(Keyboard::Key::Num7, '7')
			RETURN_KEY(Keyboard::Key::Num8, '8')
			RETURN_KEY(Keyboard::Key::Num9, '9')
			RETURN_KEY(Keyboard::Key::Escape, VK_ESCAPE)
			RETURN_KEY(Keyboard::Key::LCtrl, VK_LCONTROL)
			RETURN_KEY(Keyboard::Key::RCtrl, VK_RCONTROL)
			RETURN_KEY(Keyboard::Key::LShift, VK_LSHIFT)
			RETURN_KEY(Keyboard::Key::RShift, VK_RSHIFT)
			RETURN_KEY(Keyboard::Key::LAlt, VK_LMENU)
			RETURN_KEY(Keyboard::Key::RAlt, VK_RMENU)
			RETURN_KEY(Keyboard::Key::LSuper, VK_LWIN)
			RETURN_KEY(Keyboard::Key::RSuper, VK_RWIN)
			RETURN_KEY(Keyboard::Key::LBracket, VK_OEM_4)
			RETURN_KEY(Keyboard::Key::RBracket, VK_OEM_6)
			RETURN_KEY(Keyboard::Key::Semicolon, VK_OEM_1)
			RETURN_KEY(Keyboard::Key::Comma, VK_OEM_COMMA)
			RETURN_KEY(Keyboard::Key::Period, VK_OEM_PERIOD)
			RETURN_KEY(Keyboard::Key::Quote, VK_OEM_7)
			RETURN_KEY(Keyboard::Key::Slash, VK_OEM_2)
			RETURN_KEY(Keyboard::Key::Backslash, VK_OEM_5)
			RETURN_KEY(Keyboard::Key::Tilde, VK_OEM_3)
			RETURN_KEY(Keyboard::Key::Equals, VK_OEM_PLUS)
			RETURN_KEY(Keyboard::Key::Hyphen, VK_OEM_MINUS)
			RETURN_KEY(Keyboard::Key::Space, VK_SPACE)
			RETURN_KEY(Keyboard::Key::Return, VK_RETURN)
			RETURN_KEY(Keyboard::Key::Backspace, VK_BACK)
			RETURN_KEY(Keyboard::Key::Tab, VK_TAB)
			RETURN_KEY(Keyboard::Key::PageUp, VK_PRIOR)
			RETURN_KEY(Keyboard::Key::PageDown, VK_NEXT)
			RETURN_KEY(Keyboard::Key::End, VK_END)
			RETURN_KEY(Keyboard::Key::Home, VK_HOME)
			RETURN_KEY(Keyboard::Key::Insert, VK_INSERT)
			RETURN_KEY(Keyboard::Key::Delete, VK_DELETE)
			RETURN_KEY(Keyboard::Key::Add, VK_ADD)
			RETURN_KEY(Keyboard::Key::Subtract, VK_SUBTRACT)
			RETURN_KEY(Keyboard::Key::Multiply, VK_MULTIPLY)
			RETURN_KEY(Keyboard::Key::Divide, VK_DIVIDE)
			RETURN_KEY(Keyboard::Key::Left, VK_LEFT)
			RETURN_KEY(Keyboard::Key::Right, VK_RIGHT)
			RETURN_KEY(Keyboard::Key::Up, VK_UP)
			RETURN_KEY(Keyboard::Key::Down, VK_DOWN)
			RETURN_KEY(Keyboard::Key::F1, VK_F1)
			RETURN_KEY(Keyboard::Key::F2, VK_F2)
			RETURN_KEY(Keyboard::Key::F3, VK_F3)
			RETURN_KEY(Keyboard::Key::F4, VK_F4)
			RETURN_KEY(Keyboard::Key::F5, VK_F5)
			RETURN_KEY(Keyboard::Key::F6, VK_F6)
			RETURN_KEY(Keyboard::Key::F7, VK_F7)
			RETURN_KEY(Keyboard::Key::F8, VK_F8)
			RETURN_KEY(Keyboard::Key::F9, VK_F9)
			RETURN_KEY(Keyboard::Key::F10, VK_F10)
			RETURN_KEY(Keyboard::Key::F11, VK_F11)
			RETURN_KEY(Keyboard::Key::F12, VK_F12)
			default: {
					return 0;
				}
		}
		#undef RETURN_KEY
	}

	//Input::Input() {
	//}

	//Input::~Input() {
	//	if( Input::_instance != nullptr ) {
	//		delete Input::_instance;
	//		Input::_instance = nullptr;
	//	}
	//}

	//Input& Input::instance() {
	//	if( nullptr == Input::_instance ) {
	//		Input::_instance = new Input();
	//	}
	//	return *Input::_instance;
	//}

	//bool Input::isKeyPressed( Keyboard::Key key ) const {
	//	int win32key = 0;
	//	#define SELECT_KEY(CiriKey, WinKey) case CiriKey: {win32key=WinKey; break;}
	//	switch( key ) {
	//		SELECT_KEY(Keyboard::Key::A, 'A')
	//		SELECT_KEY(Keyboard::Key::B, 'B')
	//		SELECT_KEY(Keyboard::Key::C, 'C')
	//		SELECT_KEY(Keyboard::Key::D, 'D')
	//		SELECT_KEY(Keyboard::Key::E, 'E')
	//		SELECT_KEY(Keyboard::Key::F, 'F')
	//		SELECT_KEY(Keyboard::Key::G, 'G')
	//		SELECT_KEY(Keyboard::Key::H, 'H')
	//		SELECT_KEY(Keyboard::Key::I, 'I')
	//		SELECT_KEY(Keyboard::Key::J, 'J')
	//		SELECT_KEY(Keyboard::Key::K, 'K')
	//		SELECT_KEY(Keyboard::Key::L, 'L')
	//		SELECT_KEY(Keyboard::Key::M, 'M')
	//		SELECT_KEY(Keyboard::Key::N, 'N')
	//		SELECT_KEY(Keyboard::Key::O, 'O')
	//		SELECT_KEY(Keyboard::Key::P, 'P')
	//		SELECT_KEY(Keyboard::Key::Q, 'Q')
	//		SELECT_KEY(Keyboard::Key::R, 'R')
	//		SELECT_KEY(Keyboard::Key::S, 'S')
	//		SELECT_KEY(Keyboard::Key::T, 'T')
	//		SELECT_KEY(Keyboard::Key::U, 'U')
	//		SELECT_KEY(Keyboard::Key::V, 'V')
	//		SELECT_KEY(Keyboard::Key::W, 'W')
	//		SELECT_KEY(Keyboard::Key::X, 'X')
	//		SELECT_KEY(Keyboard::Key::Y, 'Y')
	//		SELECT_KEY(Keyboard::Key::Z, 'Z')
	//		SELECT_KEY(Keyboard::Key::Num0, '0')
	//		SELECT_KEY(Keyboard::Key::Num1, '1')
	//		SELECT_KEY(Keyboard::Key::Num2, '2')
	//		SELECT_KEY(Keyboard::Key::Num3, '3')
	//		SELECT_KEY(Keyboard::Key::Num4, '4')
	//		SELECT_KEY(Keyboard::Key::Num5, '5')
	//		SELECT_KEY(Keyboard::Key::Num6, '6')
	//		SELECT_KEY(Keyboard::Key::Num7, '7')
	//		SELECT_KEY(Keyboard::Key::Num8, '8')
	//		SELECT_KEY(Keyboard::Key::Num9, '9')
	//		SELECT_KEY(Keyboard::Key::Escape, VK_ESCAPE)
	//		SELECT_KEY(Keyboard::Key::LCtrl, VK_LCONTROL)
	//		SELECT_KEY(Keyboard::Key::RCtrl, VK_RCONTROL)
	//		SELECT_KEY(Keyboard::Key::LShift, VK_LSHIFT)
	//		SELECT_KEY(Keyboard::Key::RShift, VK_RSHIFT)
	//		SELECT_KEY(Keyboard::Key::LAlt, VK_LMENU)
	//		SELECT_KEY(Keyboard::Key::RAlt, VK_RMENU)
	//		SELECT_KEY(Keyboard::Key::LSuper, VK_LWIN)
	//		SELECT_KEY(Keyboard::Key::RSuper, VK_RWIN)
	//		SELECT_KEY(Keyboard::Key::LBracket, VK_OEM_4)
	//		SELECT_KEY(Keyboard::Key::RBracket, VK_OEM_6)
	//		SELECT_KEY(Keyboard::Key::Semicolon, VK_OEM_1)
	//		SELECT_KEY(Keyboard::Key::Comma, VK_OEM_COMMA)
	//		SELECT_KEY(Keyboard::Key::Period, VK_OEM_PERIOD)
	//		SELECT_KEY(Keyboard::Key::Quote, VK_OEM_7)
	//		SELECT_KEY(Keyboard::Key::Slash, VK_OEM_2)
	//		SELECT_KEY(Keyboard::Key::Backslash, VK_OEM_5)
	//		SELECT_KEY(Keyboard::Key::Tilde, VK_OEM_3)
	//		SELECT_KEY(Keyboard::Key::Equals, VK_OEM_PLUS)
	//		SELECT_KEY(Keyboard::Key::Hyphen, VK_OEM_MINUS)
	//		SELECT_KEY(Keyboard::Key::Space, VK_SPACE)
	//		SELECT_KEY(Keyboard::Key::Return, VK_RETURN)
	//		SELECT_KEY(Keyboard::Key::Backspace, VK_BACK)
	//		SELECT_KEY(Keyboard::Key::Tab, VK_TAB)
	//		SELECT_KEY(Keyboard::Key::PageUp, VK_PRIOR)
	//		SELECT_KEY(Keyboard::Key::PageDown, VK_NEXT)
	//		SELECT_KEY(Keyboard::Key::End, VK_END)
	//		SELECT_KEY(Keyboard::Key::Home, VK_HOME)
	//		SELECT_KEY(Keyboard::Key::Insert, VK_INSERT)
	//		SELECT_KEY(Keyboard::Key::Delete, VK_DELETE)
	//		SELECT_KEY(Keyboard::Key::Add, VK_ADD)
	//		SELECT_KEY(Keyboard::Key::Subtract, VK_SUBTRACT)
	//		SELECT_KEY(Keyboard::Key::Multiply, VK_MULTIPLY)
	//		SELECT_KEY(Keyboard::Key::Divide, VK_DIVIDE)
	//		SELECT_KEY(Keyboard::Key::Left, VK_LEFT)
	//		SELECT_KEY(Keyboard::Key::Right, VK_RIGHT)
	//		SELECT_KEY(Keyboard::Key::Up, VK_UP)
	//		SELECT_KEY(Keyboard::Key::Down, VK_DOWN)
	//		SELECT_KEY(Keyboard::Key::F1, VK_F1)
	//		SELECT_KEY(Keyboard::Key::F2, VK_F2)
	//		SELECT_KEY(Keyboard::Key::F3, VK_F3)
	//		SELECT_KEY(Keyboard::Key::F4, VK_F4)
	//		SELECT_KEY(Keyboard::Key::F5, VK_F5)
	//		SELECT_KEY(Keyboard::Key::F6, VK_F6)
	//		SELECT_KEY(Keyboard::Key::F7, VK_F7)
	//		SELECT_KEY(Keyboard::Key::F8, VK_F8)
	//		SELECT_KEY(Keyboard::Key::F9, VK_F9)
	//		SELECT_KEY(Keyboard::Key::F10, VK_F10)
	//		SELECT_KEY(Keyboard::Key::F11, VK_F11)
	//		SELECT_KEY(Keyboard::Key::F12, VK_F12)
	//	}
	//	#undef SELECT_KEY

	//	return (GetAsyncKeyState(win32key) & 0x8000) != 0;
	//}
} // ciri