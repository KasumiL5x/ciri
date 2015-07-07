#ifndef __ciri_glgraphicsdevice__
#define __ciri_glgraphicsdevice__

#include <ciri/Common.hpp>
#include CIRI_INCLUDE_PS(GLGraphicsDevice_ps)
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
		GLGraphicsDevice_ps _platform;
	};
} // ciri

#endif /* __ciri_glgraphicsdevice__ */