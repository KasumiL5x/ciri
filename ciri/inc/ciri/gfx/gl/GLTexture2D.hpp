#ifndef __ciri_gltexture2d__
#define __ciri_gltexture2d__

#include <gl/glew.h>
#include "../ITexture2D.hpp"

namespace ciri {
	class GLTexture2D : public ITexture2D {
	public:
		GLTexture2D();
		virtual ~GLTexture2D();

		virtual void destroy();
		virtual bool setData( int xOffset, int yOffset, int width, int height, void* data, TextureFormat::Type format );

		virtual int getWidth() const;
		virtual int getHeight() const;

		GLuint getTextureId() const;

	private:
		void ciriFormatToGlFormat( TextureFormat::Type ciriFormat );

	private:
		GLuint _textureId;
		GLint _internalFormat;
		GLenum _pixelFormat;
		GLenum _pixelType;
		int _width;
		int _height;
	};
} // ciri

#endif /* __ciri_gltexture2d__ */