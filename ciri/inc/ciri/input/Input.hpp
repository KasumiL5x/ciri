#ifndef __ciri_input__
#define __ciri_input__

#include <Windows.h>

namespace ciri {
	class Window;

	class Keyboard {
	public:
		enum Key {
			A=1,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,
			Num0,Num1,Num2,Num3,Num4,Num5,Num6,Num7,Num8,Num9,
			Escape,
			LCtrl,RCtrl,
			LShift,RShift,
			LAlt,RAlt,
			LSuper,RSuper,
			LBracket,RBracket,
			Semicolon,
			Comma,
			Period,
			Quote,
			Slash,
			Backslash,
			Tilde,
			Equals,
			Hyphen,
			Space,
			Return,
			Backspace,
			Tab,
			PageUp,PageDown,
			End,
			Home,
			Insert,
			Delete,
			Add,
			Subtract,
			Multiply,
			Divide,
			Left,
			Right,
			Up,
			Down,
			F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,F11,F12
		};
	};

	class MouseButton {
	public:
		enum Button {
			Left=0, Middle, Right, X1, X2, Max
		};
	};

	struct KeyboardState {
		friend class Input;

		KeyboardState();
		KeyboardState( const KeyboardState& rhs );
		KeyboardState& operator=( const KeyboardState& rhs );

		bool isKeyDown( Keyboard::Key key ) const;
		bool isKeyUp( Keyboard::Key key ) const;

	private:
		const static int KEY_ARRAY_SIZE = 256;
		static unsigned char _keyBuffer[KEY_ARRAY_SIZE]; // shared between all states; used by win32 code
		bool states[KEY_ARRAY_SIZE];
	};

	struct MouseState {
		friend class Input;

		MouseState();
		MouseState( const MouseState& rhs );
		MouseState& operator=( const MouseState& rhs );

		bool isButtonDown( MouseButton::Button button ) const;
		bool isButtonUp( MouseButton::Button button ) const;

	public:
		int x;
		int y;

	private:
		bool states[MouseButton::Max];
	};

	class Input {
	public:
		static void getKeyboardState( KeyboardState* outState );
		static void getMouseState( MouseState* outState, Window* window );
		static int ciriKeyToWinKey( Keyboard::Key key );
	};
} // ciri

#endif /* __ciri_input__ */