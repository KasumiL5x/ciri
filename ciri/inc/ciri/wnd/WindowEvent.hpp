#ifndef __ciri_wnd_window_event__
#define __ciri_wnd_window_event__

namespace ciri {
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
} // ciri

#endif /* __ciri_wnd_window_event__ */