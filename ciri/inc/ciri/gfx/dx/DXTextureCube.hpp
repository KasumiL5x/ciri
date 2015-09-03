#ifndef __ciri_dxtexturecube__
#define __ciri_dxtexturecube__

#include "../ITextureCube.hpp"

namespace ciri {
	class DXGraphicsDevice;

	class DXTextureCube : public ITextureCube {
	public:
		DXTextureCube( DXGraphicsDevice* device );
		virtual ~DXTextureCube();

		virtual err::ErrorCode set( int width, int height, void* right, void* left, void* top, void* bottom, void* back, void* front ) override;
		virtual void destroy() override;

	private:
		DXGraphicsDevice* _device;
	};
} // ciri

#endif /* __ciri_dxtexturecube__ */
