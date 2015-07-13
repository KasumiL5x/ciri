#ifndef __ciri_window_event__
#define __ciri_window_event__

namespace ciri {
	class WindowEvent {
	public:
		struct SizeEvent {
			unsigned int width;
			unsigned int height;
		};

		enum EventType {
			Closed,
			Resized, // when the window has been resized (SizeEvent),
			FocusGained, // when the window gains focus
			FocusLost, // when the window loses focus
			Count
		};

	public:
		EventType type;
		union {
			SizeEvent size; // Resized
		};
	};
}

#endif /* __ciri_window_event__ */