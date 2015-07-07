#ifndef __ciri_dxgraphicsdevice_win__
#define __ciri_dxgraphicsdevice_win__

namespace ciri {
	class Window;

	class DXGraphicsDevice_ps {
	public:
		DXGraphicsDevice_ps();
		~DXGraphicsDevice_ps();

		bool create( Window* window );
	};
} // ciri

#endif /* __ciri_dxgraphicsdevice_win__ */