#include <ciri/graphics/win/gl/GLTexture2D.hpp>
#include <ciri/graphics/win/gl/CiriToGl.hpp>
#include <ciri/core/TGA.hpp>

using namespace ciri::graphics;
using namespace ciri::core;

GLTexture2D::GLTexture2D( int flags )
	: ITexture2D(flags), _flags(flags), _format(TextureFormat::RGBA32_UINT), _textureId(0), _internalFormat(0), _pixelFormat(0), _pixelType(0), _width(0), _height(0) {
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

ErrorCode GLTexture2D::setData( int xOffset, int yOffset, int width, int height, void* data, TextureFormat::Format format ) {
	// update if already valid
	if( _textureId != 0 ) {
		// format must be the same
		if( format != _format ) {
			return ErrorCode::CIRI_INVALID_ARGUMENT;
		}

		// for now, the enture texture must be updated, and it must therefore be of the same size
		if( width != _width || height != _height || xOffset != 0 || yOffset != 0 ) {
			return ErrorCode::CIRI_NOT_IMPLEMENTED;
		}

		//
		// NOTE: updating below is completely untested
		//

		const int level = 0; // todo
		glBindTexture(GL_TEXTURE_2D, _textureId);
		glPixelStorei(GL_UNPACK_ALIGNMENT, TextureFormat::bytesPerPixel(format));
		glTexSubImage2D(GL_TEXTURE_2D, level, xOffset, yOffset, width, height, _pixelFormat, _pixelType, data);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glBindTexture(GL_TEXTURE_2D, 0);

		return ErrorCode::CIRI_OK;
	}

	// offsets must be zero when initializing the texture
	if( xOffset != 0 || yOffset != 0 ) {
		return ErrorCode::CIRI_INVALID_ARGUMENT;
	}

	// width and height must be positive
	if( width <= 0 || height <= 0 ) {
		return ErrorCode::CIRI_INVALID_ARGUMENT;
	}


	_width = width;
	_height = height;
	_format = format;

	// convert to appropriate gl formats
	ciriToGlTextureFormat(format, &_internalFormat, &_pixelFormat, &_pixelType);

	const int level = 0; // todo
	// generate and bind the texture
	glGenTextures(1, &_textureId);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	// change the pixel store to match that of the format's bytes per pixel
	glPixelStorei(GL_UNPACK_ALIGNMENT, TextureFormat::bytesPerPixel(format));
	// set the texture data
	glTexImage2D(GL_TEXTURE_2D, level, _internalFormat, width, height, 0, _pixelFormat, _pixelType, data);
	// reset pixel store back to default
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	// generate mipmaps
	if( _flags & TextureFlags::Mipmaps ) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	return ErrorCode::CIRI_OK;
}

int GLTexture2D::getWidth() const {
	return _width;
}

int GLTexture2D::getHeight() const {
	return _height;
}

TextureFormat::Format GLTexture2D::getFormat() const {
	return _format;
}

ErrorCode GLTexture2D::writeToTGA( const char* file ) {
	// todo: return ciri error codes instead of a boolean
	if( nullptr == file || 0 == _textureId ) {
		return ErrorCode::CIRI_UNKNOWN_ERROR; // todo
	}

	GLuint tmpFbo;
	glGenFramebuffers(1, &tmpFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, tmpFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureId, 0);

	// todo: what if i want to support other formats in the future like floating point textures? another function probably!
	unsigned char* pixels = new unsigned char[_width * _height * TextureFormat::bytesPerPixel(_format)];
	glReadPixels(0, 0, _width, _height, _pixelFormat, GL_UNSIGNED_BYTE, pixels); // _pixelType
	glDeleteFramebuffers(1, &tmpFbo);

	if( !TGA::writeToFile(file, _width, _height, pixels, TextureFormat::hasAlpha(_format) ? TGA::RGBA : TGA::RGB, true) ) {
		delete[] pixels;
		pixels = nullptr;
		return ErrorCode::CIRI_UNKNOWN_ERROR; // todo
	}

	delete[] pixels;
	pixels = nullptr;

	return ErrorCode::CIRI_OK;
}

ErrorCode GLTexture2D::writeToDDS( const char* file ) {
	// not supported
	return ErrorCode::CIRI_NOT_IMPLEMENTED;;
}

GLuint GLTexture2D::getTextureId() const {
	return _textureId;
}