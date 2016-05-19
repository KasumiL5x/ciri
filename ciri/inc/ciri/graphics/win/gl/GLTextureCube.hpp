#ifndef __ciri_graphics_GLTextureCube__
#define __ciri_graphics_GLTextureCube__

#include <GL/glew.h>
#include <ciri/graphics/ITextureCube.hpp>

namespace ciri {

class GLTextureCube : public ITextureCube {
public:
	GLTextureCube();
	virtual ~GLTextureCube();

	virtual ErrorCode set( int width, int height, void* posx, void* negx, void* posy, void* negy, void* posz, void* negz ) override;
	virtual void destroy() override;

	GLuint getTextureId() const;

private:
	GLuint _textureId;
};

}

#endif