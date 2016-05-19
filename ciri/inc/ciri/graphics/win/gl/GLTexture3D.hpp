#ifndef __ciri_graphics_GLTexture3D__
#define __ciri_graphics_GLTexture3D__

#include <GL/glew.h>
#include <ciri/graphics/ITexture3D.hpp>

namespace ciri { namespace graphics {

class GLTexture3D : public ITexture3D {
public:
	GLTexture3D( int flags );
	virtual ~GLTexture3D();

	virtual void destroy() override;
	virtual core::ErrorCode setData( int width, int height, int depth, void* data, TextureFormat::Format format ) override;

	virtual int getWidth() const override;
	virtual int getHeight() const override;
	virtual int getDepth() const override;
	virtual TextureFormat::Format getFormat() const override;

	virtual core::ErrorCode writeToTGA( const char* file ) override;
	virtual core::ErrorCode writeToDDS( const char* file ) override;

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
	int _depth;
};

}}

#endif