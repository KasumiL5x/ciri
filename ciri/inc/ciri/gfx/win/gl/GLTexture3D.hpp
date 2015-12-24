#ifndef __ciri_gfx_gltexture3d__
#define __ciri_gfx_gltexture3d__

#include <GL/glew.h>
#include <ciri/gfx/ITexture3D.hpp>

namespace ciri {
	class GLTexture3D : public ITexture3D {
	public:
		GLTexture3D( int flags );
		virtual ~GLTexture3D();

		virtual void destroy() override;
		virtual ErrorCode setData( int width, int height, int depth, void* data, TextureFormat::Format format ) override;

		virtual int getWidth() const override;
		virtual int getHeight() const override;
		virtual int getDepth() const override;
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
		int _depth;
	};
} // ciri

#endif /* __ciri_gfx_gltexture3d__ */