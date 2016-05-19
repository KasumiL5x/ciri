#ifndef __ciri_core_WindowEvent__
#define __ciri_core_WindowEvent__

namespace ciri { namespace core {

class WindowEvent {
public:
	struct SizeEvent {
		unsigned int width;
		unsigned int height;
	};

	struct KeyEvent {
		int code;
		bool alt;
		bool ctrl;
		bool shift;
		bool super;
	};

	enum EventType {
		Closed,
		Resized, // when the window has been resized (SizeEvent),
		FocusGained, // when the window gains focus
		FocusLost, // when the window loses focus,
		KeyDown, // a key was pressed down
		KeyUp, // a pressed down key was released up
		Count
	};

public:
	EventType type;
	union {
		SizeEvent size; // Resized
		KeyEvent key; // KeyDown, KeyUp
	};
};

}}

#endif