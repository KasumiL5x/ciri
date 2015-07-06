#ifndef __ciri_window_event__
#define __ciri_window_event__

namespace ciri {
	class WindowEvent {
	public:
		enum EventType {
			Closed,
			Count
		};

	public:
		EventType type;
	};
}

#endif /* __ciri_window_event__ */