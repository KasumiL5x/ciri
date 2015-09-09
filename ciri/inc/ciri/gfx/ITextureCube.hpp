#ifndef __ciri_itexturecube__
#define __ciri_itexturecube__

#include "../core/ErrorCodes.hpp"

namespace ciri {
	class ITextureCube {
	public:
		ITextureCube() {
		}
		virtual ~ITextureCube() {
		}

		virtual err::ErrorCode set( int width, int height, void* posx, void* negx, void* posy, void* negy, void* posz, void* negz )=0;

		virtual void destroy()=0;
	};
} // ciri

#endif/* __ciri_itexturecube__ */
