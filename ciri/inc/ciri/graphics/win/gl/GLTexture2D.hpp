#ifndef __ciri_graphics_GLTexture2D__
#define __ciri_graphics_GLTexture2D__

#include <GL/glew.h>
#include <ciri/graphics/ITexture2D.hpp>

namespace ciri {

class GLTexture2D : public ITexture2D {
public:
	GLTexture2D( int flags );
	virtual ~GLTexture2D();

	virtual void destroy() override;
	virtual ErrorCode setData( int xOffset, int yOffset, int width, int height, void* data, TextureFormat::Format format ) override;

	virtual int getWidth() const override;
	virtual int getHeight() const override;
	virtual TextureFormat::Format getFormat() const override;

	virtual ErrorCode writeToTGA( const char* file ) override;
	virtual ErrorCode writeToDDS( const char* file ) override;

	GLuint getTextureId() const;

private:
	int _flags;
	TextureFormat::Format _format;
	GLuint _textureId;
	GLint _internalFormat;
	GLenum _pixelFormat;
	GLenum _pixelType;
	int _width;
	int _height;
};

}

#endif