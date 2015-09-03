#ifndef __ciri_gltexturecube__
#define __ciri_gltexturecube__

#include "../ITextureCube.hpp"

namespace ciri {
	class GLTextureCube : public ITextureCube {
	public:
		GLTextureCube();
		virtual ~GLTextureCube();

		virtual err::ErrorCode set( int width, int height, void* right, void* left, void* top, void* bottom, void* back, void* front ) override;
		virtual void destroy() override;
	};
} // ciri

#endif /* __ciri_gltexturecube__ */
