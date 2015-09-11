#include <ciri/input/win/Input.hpp>
#include <ciri/wnd/IWindow.hpp>

namespace ciri {
	unsigned char KeyboardState::_keyBuffer[KeyboardState::KEY_ARRAY_SIZE];
	static int ciriKeyToWinKey( Key key ) {
		#define RETURN_KEY(Ciri, Win) case Ciri: { return Win; }
		switch( key ) {
			RETURN_KEY(Key::A, 'A')
			RETURN_KEY(Key::B, 'B')
			RETURN_KEY(Key::C, 'C')
			RETURN_KEY(Key::D, 'D')
			RETURN_KEY(Key::E, 'E')
			RETURN_KEY(Key::F, 'F')
			RETURN_KEY(Key::G, 'G')
			RETURN_KEY(Key::H, 'H')
			RETURN_KEY(Key::I, 'I')
			RETURN_KEY(Key::J, 'J')
			RETURN_KEY(Key::K, 'K')
			RETURN_KEY(Key::L, 'L')
			RETURN_KEY(Key::M, 'M')
			RETURN_KEY(Key::N, 'N')
			RETURN_KEY(Key::O, 'O')
			RETURN_KEY(Key::P, 'P')
			RETURN_KEY(Key::Q, 'Q')
			RETURN_KEY(Key::R, 'R')
			RETURN_KEY(Key::S, 'S')
			RETURN_KEY(Key::T, 'T')
			RETURN_KEY(Key::U, 'U')
			RETURN_KEY(Key::V, 'V')
			RETURN_KEY(Key::W, 'W')
			RETURN_KEY(Key::X, 'X')
			RETURN_KEY(Key::Y, 'Y')
			RETURN_KEY(Key::Z, 'Z')
			RETURN_KEY(Key::Num0, '0')
			RETURN_KEY(Key::Num1, '1')
			RETURN_KEY(Key::Num2, '2')
			RETURN_KEY(Key::Num3, '3')
			RETURN_KEY(Key::Num4, '4')
			RETURN_KEY(Key::Num5, '5')
			RETURN_KEY(Key::Num6, '6')
			RETURN_KEY(Key::Num7, '7')
			RETURN_KEY(Key::Num8, '8')
			RETURN_KEY(Key::Num9, '9')
			RETURN_KEY(Key::Escape, VK_ESCAPE)
			RETURN_KEY(Key::LCtrl, VK_LCONTROL)
			RETURN_KEY(Key::RCtrl, VK_RCONTROL)
			RETURN_KEY(Key::LShift, VK_LSHIFT)
			RETURN_KEY(Key::RShift, VK_RSHIFT)
			RETURN_KEY(Key::LAlt, VK_LMENU)
			RETURN_KEY(Key::RAlt, VK_RMENU)
			RETURN_KEY(Key::LSuper, VK_LWIN)
			RETURN_KEY(Key::RSuper, VK_RWIN)
			RETURN_KEY(Key::LBracket, VK_OEM_4)
			RETURN_KEY(Key::RBracket, VK_OEM_6)
			RETURN_KEY(Key::Semicolon, VK_OEM_1)
			RETURN_KEY(Key::Comma, VK_OEM_COMMA)
			RETURN_KEY(Key::Period, VK_OEM_PERIOD)
			RETURN_KEY(Key::Quote, VK_OEM_7)
			RETURN_KEY(Key::Slash, VK_OEM_2)
			RETURN_KEY(Key::Backslash, VK_OEM_5)
			RETURN_KEY(Key::Tilde, VK_OEM_3)
			RETURN_KEY(Key::Equals, VK_OEM_PLUS)
			RETURN_KEY(Key::Hyphen, VK_OEM_MINUS)
			RETURN_KEY(Key::Space, VK_SPACE)
			RETURN_KEY(Key::Return, VK_RETURN)
			RETURN_KEY(Key::Backspace, VK_BACK)
			RETURN_KEY(Key::Tab, VK_TAB)
			RETURN_KEY(Key::PageUp, VK_PRIOR)
			RETURN_KEY(Key::PageDown, VK_NEXT)
			RETURN_KEY(Key::End, VK_END)
			RETURN_KEY(Key::Home, VK_HOME)
			RETURN_KEY(Key::Insert, VK_INSERT)
			RETURN_KEY(Key::Delete, VK_DELETE)
			RETURN_KEY(Key::Add, VK_ADD)
			RETURN_KEY(Key::Subtract, VK_SUBTRACT)
			RETURN_KEY(Key::Multiply, VK_MULTIPLY)
			RETURN_KEY(Key::Divide, VK_DIVIDE)
			RETURN_KEY(Key::Left, VK_LEFT)
			RETURN_KEY(Key::Right, VK_RIGHT)
			RETURN_KEY(Key::Up, VK_UP)
			RETURN_KEY(Key::Down, VK_DOWN)
			RETURN_KEY(Key::F1, VK_F1)
			RETURN_KEY(Key::F2, VK_F2)
			RETURN_KEY(Key::F3, VK_F3)
			RETURN_KEY(Key::F4, VK_F4)
			RETURN_KEY(Key::F5, VK_F5)
			RETURN_KEY(Key::F6, VK_F6)
			RETURN_KEY(Key::F7, VK_F7)
			RETURN_KEY(Key::F8, VK_F8)
			RETURN_KEY(Key::F9, VK_F9)
			RETURN_KEY(Key::F10, VK_F10)
			RETURN_KEY(Key::F11, VK_F11)
			RETURN_KEY(Key::F12, VK_F12)
			default: {
					return 0;
				}
		}
		#undef RETURN_KEY
	}

	KeyboardState::KeyboardState() {
	}

	KeyboardState::KeyboardState( const KeyboardState& rhs ) {
		memcpy(this->states, rhs.states, sizeof(bool) * KeyboardState::KEY_ARRAY_SIZE);
	}

	KeyboardState& KeyboardState::operator=( const KeyboardState& rhs ) {
		memcpy(this->states, rhs.states, sizeof(bool) * KeyboardState::KEY_ARRAY_SIZE);
		return *this;
	}

	MouseState::MouseState() {
	}

	MouseState::MouseState( const MouseState& rhs ) {
		memcpy(this->states, rhs.states, sizeof(bool) * (int)MouseButton::Max);
		this->x = rhs.x;
		this->y = rhs.y;
	}

	MouseState& MouseState::operator=( const MouseState& rhs ) {
		memcpy(this->states, rhs.states, sizeof(bool) * (int)MouseButton::Max);
		this->x = rhs.x;
		this->y = rhs.y;
		return *this;
	}

	Input::Input()
		: IInput(), _window(nullptr) {
	}

	Input::~Input() {
	}

	bool Input::create( std::shared_ptr<IWindow> window ) {
		if( nullptr == window ) {
			return false;
		}

		_window = window;
		return true;
	}

	bool Input::poll() {
		if( nullptr == _window ) {
			return false;
		}

		// get current keyboard state
		GetKeyboardState(KeyboardState::_keyBuffer);
		for( int i = 0; i < KeyboardState::KEY_ARRAY_SIZE; ++i ) {
			_currKeyState.states[i] = (KeyboardState::_keyBuffer[i] & 0x80) ? true : false;
		}

		// get current mouse state
		_currMouseState.states[(int)MouseButton::Left] = (GetKeyState(VK_LBUTTON) & 0x80) ? true : false;
		_currMouseState.states[(int)MouseButton::Middle] = (GetKeyState(VK_MBUTTON) & 0x80) ? true : false;
		_currMouseState.states[(int)MouseButton::Right] = (GetKeyState(VK_RBUTTON) & 0x80) ? true : false;
		_currMouseState.states[(int)MouseButton::X1] = (GetKeyState(VK_XBUTTON1) & 0x80) ? true : false;
		_currMouseState.states[(int)MouseButton::X2] = (GetKeyState(VK_XBUTTON2) & 0x80) ? true : false;
		POINT pos;
		GetCursorPos(&pos);
		ScreenToClient(static_cast<HWND>(_window->getNativeHandle()), &pos);
		_currMouseState.x = static_cast<int>(pos.x);
		_currMouseState.y = static_cast<int>(pos.y);

		return true;
	}

	bool Input::update() {
		if( nullptr == _window ) {
			return false;
		}

		// update previous keyboard state
		_prevKeyState = _currKeyState;

		// update previous mouse state
		_prevMouseState = _currMouseState;

		return true;
	}

	bool Input::isKeyDown( Key key ) const {
		return _currKeyState.states[ciriKeyToWinKey(key)];
	}

	bool Input::isKeyUp( Key key ) const {
		return !isKeyDown(key);
	}

	bool Input::wasKeyDown( Key key ) const {
		return _prevKeyState.states[ciriKeyToWinKey(key)];
	}

	bool Input::wasKeyUp( Key key ) const {
		return !wasKeyDown(key);
	}

	bool Input::isMouseButtonDown( MouseButton button ) const {
		return _currMouseState.states[(int)button];
	}

	bool Input::isMouseButtonUp( MouseButton button ) const {
		return !isMouseButtonDown(button);
	}

	bool Input::wasMouseButtonDown( MouseButton button ) const {
		return _prevMouseState.states[(int)button];
	}

	bool Input::wasMouseButtonUp( MouseButton button ) const {
		return !wasMouseButtonDown(button);
	}

	int Input::mouseX() const {
		return _currMouseState.x;
	}

	int Input::mouseY() const {
		return _currMouseState.y;
	}

	int Input::lastMouseX() const {
		return _prevMouseState.x;
	}

	int Input::lastMouseY() const {
		return _prevMouseState.y;
	}

	std::shared_ptr<IInput> createInput() {
		return std::shared_ptr<IInput>(new Input());
	}
} // ciri
