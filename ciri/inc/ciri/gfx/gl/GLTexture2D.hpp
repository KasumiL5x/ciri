#ifndef __ciri_gltexture2d__
#define __ciri_gltexture2d__

#include <gl/glew.h>
#include "../ITexture2D.hpp"

namespace ciri {
	class GLTexture2D : public ITexture2D {
	public:
		GLTexture2D( int flags );
		virtual ~GLTexture2D();

		virtual void destroy();
		virtual err::ErrorCode setData( int xOffset, int yOffset, int width, int height, void* data, TextureFormat::Format format );

		virtual int getWidth() const;
		virtual int getHeight() const;

		virtual err::ErrorCode writeToTGA( const char* file );
		virtual err::ErrorCode writeToDDS( const char* file );

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
} // ciri

#endif /* __ciri_gltexture2d__ */