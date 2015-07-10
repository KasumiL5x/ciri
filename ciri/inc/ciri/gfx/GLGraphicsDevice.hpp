#ifndef __ciri_glgraphicsdevice__
#define __ciri_glgraphicsdevice__

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32s.lib")

#include <Windows.h>
#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "IGraphicsDevice.hpp"

namespace ciri {
	class GLGraphicsDevice : public IGraphicsDevice {
	public:
		GLGraphicsDevice();
		virtual ~GLGraphicsDevice();

		virtual bool create( Window* window );
		virtual void destroy();
		virtual void present();

	private:
		bool configureGl( HWND hwnd );
		bool configureGlew();

	private:
		HDC _hdc;
		HGLRC _hglrc;
	};
} // ciri

#endif /* __ciri_glgraphicsdevice__ */