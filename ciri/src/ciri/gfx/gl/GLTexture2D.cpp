#include <ciri/gfx/gl/GLTexture2D.hpp>

namespace ciri {
	GLTexture2D::GLTexture2D()
		: ITexture2D(), _textureId(0), _internalFormat(0), _pixelFormat(0), _pixelType(0) {
	}

	GLTexture2D::~GLTexture2D() {
		destroy();
	}

	void GLTexture2D::destroy() {
		if( _textureId != 0 ) {
			glDeleteTextures(1, &_textureId);
			_textureId = 0;
		}
	}

	bool GLTexture2D::setData( int xOffset, int yOffset, int width, int height, void* data, TextureFormat::Type format ) {
		ciriFormatToGlFormat(format);

		const GLint minFilterMode = (1 > 1) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR; // (levelCount > 1) ? ... : ...;
		const int level = 0;
		GLint wrapMode = GL_REPEAT;
		// clamp wrap mode to edge if not power of two
		if( ((width & (width - 1)) != 0) || ((height & (height - 1)) != 0) ) {
			wrapMode = GL_CLAMP_TO_EDGE;
		}

		if( _textureId != 0 ) {
			glBindTexture(GL_TEXTURE_2D, _textureId);
			glPixelStorei(GL_UNPACK_ALIGNMENT, getPixelStoreSize(format));
			glTexSubImage2D(GL_TEXTURE_2D, level, xOffset, yOffset, width, height, _pixelFormat, _pixelType, data);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			glBindTexture(GL_TEXTURE_2D, 0);
		} else {
			if( xOffset != 0 || yOffset != 0 ) {
				return false; // todo: error (cannot set a nonzeroed rect on a blank image)
			}

			glGenTextures(1, &_textureId);
			glBindTexture(GL_TEXTURE_2D, _textureId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilterMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
			glPixelStorei(GL_UNPACK_ALIGNMENT, getPixelStoreSize(format));
			glTexImage2D(GL_TEXTURE_2D, level, _internalFormat, width, height, 0, _pixelFormat, _pixelType, data);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		return true;
	}

	void GLTexture2D::ciriFormatToGlFormat( TextureFormat::Type ciriFormat ) {
		// http://www.opentk.com/files/doc/namespace_open_t_k_1_1_graphics_1_1_open_g_l.html#ae0f3f1e7b978e4937984b34fdebabf62a8f0fb883eb5a52838534191513e365a2
		// MonoGame.Framework.Graphics.GraphicsExtensions.cs: GetGLFormat
		// todo: change rgb to rgba and fix the no alpha bug (add another format?)
		_internalFormat = GL_RGB;
		_pixelFormat = GL_RGB;
		_pixelType = GL_UNSIGNED_BYTE;

		switch( ciriFormat ) {
			case TextureFormat::Color: {
				_internalFormat = GL_RGB;
				_pixelFormat = GL_RGB;
				_pixelType = GL_UNSIGNED_BYTE;
				break;
			}
		}
	}

	int GLTexture2D::getPixelStoreSize( TextureFormat::Type format ) {
		// MonoGame.Framework.Graphics.GraphicsExtensions.cs: GetSize
		switch( format ) {
			case TextureFormat::Color: {
				return 4;
			}

			default: {
				return -1;
			}
		}
	}
} // ciri