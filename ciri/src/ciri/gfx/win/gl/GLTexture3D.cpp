#include <ciri/gfx/win/gl/GLTexture3D.hpp>
#include <ciri/gfx/win/gl/CiriToGl.hpp>

namespace ciri {
	GLTexture3D::GLTexture3D( int flags )
		: ITexture3D(flags), _flags(flags), _format(TextureFormat::RGBA32_UINT), _textureId(0), _internalFormat(0), _pixelFormat(0), _pixelType(0), _width(0), _height(0), _depth(0) {
	}

	GLTexture3D::~GLTexture3D() {
	}

	void GLTexture3D::destroy() {
		if( _textureId != 0 ) {
			glDeleteTextures(1, &_textureId);
			_textureId = 0;
		}
	}
	
	ErrorCode GLTexture3D::setData( int width, int height, int depth, void* data, TextureFormat::Format format ) {
		// update if already valid
		if( _textureId != 0 ) {
			return ErrorCode::CIRI_NOT_IMPLEMENTED;
		}

		// all dimensions must be positive
		if( width <= 0 || height <= 0 || depth <= 0 ) {
			return ErrorCode::CIRI_INVALID_ARGUMENT;
		}

		// store dimensions
		_width = width;
		_height = height;
		_depth = depth;

		// convert to gl formats
		ciriToGlTextureFormat(format, &_internalFormat, &_pixelFormat, &_pixelType);

		// generate and bind the texture
		glGenTextures(1, &_textureId);
		glBindTexture(GL_TEXTURE_3D, _textureId);
		// change the pixel store to match the format's bytes per pixel
		glPixelStorei(GL_UNPACK_ALIGNMENT, TextureFormat::bytesPerPixel(format));
		// set the texture data
		glTexImage3D(GL_TEXTURE_3D, 0, _internalFormat, width, height, depth, 0, _pixelFormat, _pixelType, data);
		// resetr pixel store back to default
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		// generate mipmaps
		if( _flags & TextureFlags::Mipmaps ) {
			glGenerateMipmap(GL_TEXTURE_3D);
		}

		// unbind texture
		glBindTexture(GL_TEXTURE_3D, 0);

		return ErrorCode::CIRI_OK;
	}
	
	int GLTexture3D::getWidth() const {
		return _width;
	}
	
	int GLTexture3D::getHeight() const {
		return _height;
	}
	
	int GLTexture3D::getDepth() const {
		return _depth;
	}
	
	TextureFormat::Format GLTexture3D::getFormat() const {
		return _format;
	}
	
	ErrorCode GLTexture3D::writeToTGA( const char* file ) {
		return ErrorCode::CIRI_NOT_IMPLEMENTED;
	}
	
	ErrorCode GLTexture3D::writeToDDS( const char* file ) {
		return ErrorCode::CIRI_NOT_IMPLEMENTED;
	}
	
	GLuint GLTexture3D::getTextureId() const {
		return _textureId;
	}

} // ciri
