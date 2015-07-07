#ifndef __ciri_graphics_device_win_dx__
#define __ciri_graphics_device_win_dx__

namespace ciri {
	class Window;

	class GraphicsDevice_ps {
	public:
		GraphicsDevice_ps();
		~GraphicsDevice_ps();

		bool create( Window* window );
	};
} // ciri

#endif /* __ciri_graphics_device_win_dx__ */