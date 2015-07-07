#ifndef __ciri_window_win__
#define __ciri_window_win__

#include <queue>
#include <Windows.h>
#include "../WindowEvent.hpp"
#include <cc/Vec2.hpp>

namespace ciri {
	class Window_ps {
	public:
		Window_ps();
		~Window_ps();

		bool create(int width, int height);
		bool isOpen() const;
		bool pollEvent( WindowEvent& evt );
		void destroy();
		cc::Vec2ui getSize() const;
		void* getHandle() const;

	private:
		bool createWindow( int width, int height );
		void processEvents();
		void processEvent( UINT msg, WPARAM wparam, LPARAM lparam );
		void pushEvent( WindowEvent& evt );
		static LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );

	private:
		HWND _hwnd;
		std::queue<WindowEvent> _events;
		bool _resizing;
		cc::Vec2ui _lastSize;
	};
}

#endif /* __ciri_window_win__ */