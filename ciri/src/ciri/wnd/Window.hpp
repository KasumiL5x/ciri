#ifndef __ciri_window__
#define __ciri_window__

#include <ciri/Common.hpp>
#include CIRI_INCLUDE_PS(Window_ps)
#include "WindowEvent.hpp"
#include <cc/Vec2.hpp>

namespace ciri {
	class Window{
	public:
		Window();
		~Window();

		bool create( int width, int height );
		bool isOpen() const;
		bool pollEvent( WindowEvent& evt );
		void destroy();

		cc::Vec2ui getSize() const;

	private:
		Window_ps _platform;
	};
} // ciri

#endif /* __ciri_window__ */