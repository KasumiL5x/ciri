#ifndef __ciri_gltexturecube__
#define __ciri_gltexturecube__

#include <gl/glew.h>
#include "../ITextureCube.hpp"

namespace ciri {
	class GLTextureCube : public ITextureCube {
	public:
		GLTextureCube();
		virtual ~GLTextureCube();

		virtual err::ErrorCode set( int width, int height, void* posx, void* negx, void* posy, void* negy, void* posz, void* negz ) override;
		virtual void destroy() override;

		GLuint getTextureId() const;

	private:
		GLuint _textureId;
	};
} // ciri

#endif /* __ciri_gltexturecube__ */
