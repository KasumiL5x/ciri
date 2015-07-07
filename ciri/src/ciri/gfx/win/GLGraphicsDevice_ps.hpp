#ifndef __ciri_glgraphicsdevice_win__
#define __ciri_glgraphicsdevice_win__

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32s.lib")

#include <Windows.h>
#include <gl/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>

namespace ciri {
	class Window;

	class GLGraphicsDevice_ps {
	public:
		GLGraphicsDevice_ps();
		~GLGraphicsDevice_ps();

		bool create( Window* window );
		void destroy();
		void present();

	private:
		bool configureGl( HWND hwnd );
		bool configureGlew();

	private:
		HDC _hdc;
		HGLRC _hglrc;
	};
} // ciri

#endif /* __ciri_glgraphicsdevice_win__ */