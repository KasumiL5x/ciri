#include <ciri/gfx/GLGraphicsDevice.hpp>
#include <ciri/wnd/Window.hpp>

namespace ciri {
	GLGraphicsDevice::GLGraphicsDevice()
		: IGraphicsDevice(), _hdc(0), _hglrc(0) {
	}

	GLGraphicsDevice::~GLGraphicsDevice() {
		destroy();
	}

	bool GLGraphicsDevice::create( Window* window ) {
		if( !configureGl(window->getHandle()) ) {
			destroy();
			return false;
		}

		if( !configureGlew() ) {
			destroy();
			return false;
		}

		return true;
	}

	void GLGraphicsDevice::destroy() {
		// make rendering context not current
		wglMakeCurrent(0, 0);

		// delete the gl context
		if( _hglrc ) { wglDeleteContext(_hglrc); _hglrc = 0; }

		// delete device context
		if( _hdc ) { DeleteDC(_hdc); _hdc = 0; }
	}

	void GLGraphicsDevice::present() {
		glClearColor(0.39f, 0.58f, 0.93f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		SwapBuffers(_hdc);
	}

	bool GLGraphicsDevice::configureGl( HWND hwnd ) {
		// get the window's device context
		_hdc = GetDC(hwnd);

		// create pixel format descriptor
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA, // The kind of framebuffer. RGBA or palette.
			32, // Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24, // Number of bits for the depthbuffer
			8, // Number of bits for the stencilbuffer
			0, // Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};
		// set pixel format of the device context
		const int pixelFormat = ChoosePixelFormat(_hdc, &pfd);
		if( 0 == pixelFormat ) {
			return false;
		}
		if( !SetPixelFormat(_hdc, pixelFormat, &pfd) ) {
			return false;
		}

		// create the opengl context
		_hglrc = wglCreateContext(_hdc);
		if( !_hglrc ) {
			return false;
		}

		// make the opengl context current
		if( !wglMakeCurrent(_hdc, _hglrc) ) {
			return false;
		}

		//TODO: setup gl debug message tracing

		return true;
	}

	bool GLGraphicsDevice::configureGlew() {
		return (glewInit() == GLEW_OK);
	}
} // ciri