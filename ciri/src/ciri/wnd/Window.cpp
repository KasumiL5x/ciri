#include <ciri/wnd/Window.hpp>

namespace ciri {
	Window::Window()
		: _hwnd(0), _resizing(false), _lastSize(0, 0), _keyRepeatEnabled(true) {
	}

	Window::~Window() {
		destroy(); // safety first, kids
	}

	bool Window::create( int width, int height ) {
		if( !createWindow(width, height) ) {
			return false;
		}

		_lastSize = cc::Vec2ui(width, height);

		return true;
	}

	bool Window::isOpen() const {
		return (_hwnd != 0);
	}

	bool Window::pollEvent( WindowEvent& evt ) {
		// if there's no events, check for new events
		if( _events.empty() ) {
			processEvents();
		}

		// pop first event, if it exists
		if( !_events.empty() ) {
			evt = _events.front();
			_events.pop();

			return true;
		}

		return false;
	}

	void Window::destroy() {
		DestroyWindow(_hwnd);
		_hwnd = 0;
	}

	cc::Vec2ui Window::getSize() const {
		RECT rc;
		GetClientRect(_hwnd, &rc);
		return cc::Vec2ui(rc.right - rc.left, rc.bottom - rc.top);
	}

	HWND Window::getHandle() const {
		return _hwnd;
	}

	bool Window::createWindow( int width, int height ) {
		// register class
		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetModuleHandle(NULL);
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = "CIRI";
		if( !RegisterClassEx(&wc) ) {
			return false;
		}

		// get actual dimensions
		RECT rc = { 0, 0, width, height };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		// create the window
		_hwnd = CreateWindow("CIRI", "CIRI", WS_OVERLAPPEDWINDOW,
							 CW_USEDEFAULT, CW_USEDEFAULT,
							 rc.right - rc.left, rc.bottom - rc.top,
							 NULL, NULL, GetModuleHandle(NULL), this);
		if( !_hwnd ) {
			return false;
		}

		// show the window
		ShowWindow(_hwnd, SW_SHOW);

		return true;
	}

	void Window::processEvents() {
		MSG msg;
		while( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void Window::processEvent( UINT msg, WPARAM wparam, LPARAM lparam ) {
		// don't process until window is created
		if( NULL == _hwnd ) {
			return;
		}

		switch( msg ) {
			case WM_DESTROY: {
				destroy();
				break;
			}

			case WM_CLOSE: {
				WindowEvent evt;
				evt.type = WindowEvent::Closed;
				pushEvent(evt);
				break;
			}

			case WM_SIZE: {
				// only consider events triggered by maximimze or unmaximize
				if( wparam != SIZE_MINIMIZED && !_resizing && _lastSize != getSize() ) {
					_lastSize = getSize();

					// push resize event
					WindowEvent evt;
					evt.type = WindowEvent::Resized;
					evt.size.width = _lastSize.x;
					evt.size.height = _lastSize.y;
					pushEvent(evt);
				}
				break;
			}

			// start resizing
			case WM_ENTERSIZEMOVE: {
				_resizing = true;
				break;
			}

			// stop resizing
			case WM_EXITSIZEMOVE: {
				_resizing = false;

				// ignore cases where window has moved (size doesn't change)
				if( _lastSize != getSize() ) {
					_lastSize = getSize();

					// push resize event
					WindowEvent evt;
					evt.type = WindowEvent::Resized;
					evt.size.width = _lastSize.x;
					evt.size.height = _lastSize.y;
					pushEvent(evt);
				}
				break;
			}

				// gained focus
			case WM_SETFOCUS: {
				WindowEvent evt;
				evt.type = WindowEvent::FocusGained;
				pushEvent(evt);
				break;
			}

			// lost focus
			case WM_KILLFOCUS: {
				WindowEvent evt;
				evt.type = WindowEvent::FocusLost;
				pushEvent(evt);
				break;
			}

			// character input
			case WM_CHAR: {
				break;
			}

			// keydown
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN: {
				if( _keyRepeatEnabled || ((HIWORD(lparam) & KF_REPEAT) == 0) ) {
					WindowEvent evt;
					evt.type = WindowEvent::KeyDown;
					evt.key.alt = HIWORD(GetAsyncKeyState(VK_MENU)) != 0;
					evt.key.ctrl = HIWORD(GetAsyncKeyState(VK_CONTROL)) != 0;
					evt.key.shift = HIWORD(GetAsyncKeyState(VK_SHIFT)) != 0;
					evt.key.super = (HIWORD(GetAsyncKeyState(VK_LWIN)) != 0) || (HIWORD(GetAsyncKeyState(VK_RWIN)) != 0);
					evt.key.code = wparam;
					pushEvent(evt);
				}
				break;
			}

			// keyup
			case WM_KEYUP:
			case WM_SYSKEYUP: {
					WindowEvent evt;
					evt.type = WindowEvent::KeyUp;
					evt.key.alt = HIWORD(GetAsyncKeyState(VK_MENU)) != 0;
					evt.key.ctrl = HIWORD(GetAsyncKeyState(VK_CONTROL)) != 0;
					evt.key.shift = HIWORD(GetAsyncKeyState(VK_SHIFT)) != 0;
					evt.key.super = (HIWORD(GetAsyncKeyState(VK_LWIN)) != 0) || (HIWORD(GetAsyncKeyState(VK_RWIN)) != 0);
					evt.key.code = wparam;
					pushEvent(evt);
				break;
			}
		}
	}

	void Window::pushEvent( WindowEvent& evt ) {
		_events.push(evt);
	}

	LRESULT CALLBACK Window::WndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam ) {
		// when creating the window, associate the window handle and instance together using userdata
		if( WM_CREATE == msg ) {
			LONG_PTR window = (LONG_PTR)reinterpret_cast<CREATESTRUCT*>(lparam)->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, window);
		}

		// get the window pointer
		Window* window = hwnd ? reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)) : nullptr;

		// hand the event to the window to be processed
		if( window ) {
			window->processEvent(msg, wparam, lparam);
		}

		// don't forward wm_close to prevent OS from auto destroying the window
		if( WM_CLOSE == msg ) {
			return 0;
		}

		// don't forward syscommand to prevent alt or f10 from stealing focus
		if( (WM_SYSCOMMAND == msg) && (SC_KEYMENU == wparam) ) {
			return 0;
		}

		// let windows handle it
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

} // ciri