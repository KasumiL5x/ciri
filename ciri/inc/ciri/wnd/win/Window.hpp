#ifndef __ciri_wnd_win_window__
#define __ciri_wnd_win_window__

#include <queue>
#include <ciri/wnd/IWindow.hpp>
#include <Windows.h>

namespace ciri {
	class Window : public IWindow {
	public:
		Window();
		virtual ~Window();

		virtual bool create( int width, int height ) override;
		virtual bool isOpen() const override;
		virtual bool pollEvent( WindowEvent& evt ) override;
		virtual void close() override;
		virtual void setWindowText( const char* str ) override;
		virtual int getWidth() const override;
		virtual int getHeight() const override;
		virtual bool hasFocus() const override;
		virtual void* getNativeHandle() const override;
		virtual void setCursorVisible( bool visible ) override;

	private:
		bool createWindow( int width, int height );
		void processEvents();
		void processEvent( UINT msg, WPARAM wparam, LPARAM lparam );
		void pushEvent( WindowEvent& evt );
		static LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );

	private:
		HWND _hwnd;
		std::queue<WindowEvent> _events;
		bool _isResizing;
		int _lastWidth;
		int _lastHeight;
		bool _keyRepeatEnabled;
		bool _isCursorVisible;
	};
}

#endif /* __ciri_wnd_win_window__ */
