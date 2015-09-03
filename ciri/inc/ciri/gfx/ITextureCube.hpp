#ifndef __ciri_itexturecube__
#define __ciri_itexturecube__

#include "../ErrorCodes.hpp"

namespace ciri {
	class ITextureCube {
	public:
		ITextureCube() {
		}
		virtual ~ITextureCube() {
		}

		virtual err::ErrorCode set( int width, int height, void* right, void* left, void* top, void* bottom, void* back, void* front )=0;

		virtual void destroy()=0;
	};
} // ciri

#endif/* __ciri_itexturecube__ */
