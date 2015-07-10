#ifndef __ciri_igraphicsdevice__
#define __ciri_igraphicsdevice__

namespace ciri {
	class Window;

	class IGraphicsDevice {
	public:
		IGraphicsDevice() {
		}

		virtual ~IGraphicsDevice() {
		}

		virtual bool create( Window* window )=0;

		virtual void destroy()=0;

		virtual void present()=0;
	};
} // ciri

#endif /* __ciri_igraphicsdevice__ */