#include <ciri/graphics/win/gl/GLTextureCube.hpp>

using namespace ciri::graphics;
using namespace ciri::core;

GLTextureCube::GLTextureCube()
	: ITextureCube(), _textureId(0) {
}

GLTextureCube::~GLTextureCube() {
	destroy();
}

ErrorCode GLTextureCube::set( int width, int height, void* posx, void* negx, void* posy, void* negy, void* posz, void* negz ) {
	// http://antongerdelan.net/opengl/cubemaps.html
	// https://www.opengl.org/wiki/Cubemap_Texture
	// http://learnopengl.com/#!Advanced-OpenGL/Cubemaps

	// must have positive dimensions
	if( width <= 0 || height <= 0 ) {
		return ErrorCode::CIRI_INVALID_ARGUMENT;
	}

	// must have valid images
	if( nullptr==posx || nullptr==negx || nullptr==posy || nullptr==negy || nullptr==posz || nullptr==negz ) {
		return ErrorCode::CIRI_INVALID_ARGUMENT;
	}

	// todo: update if already valid
	if( _textureId != 0 ) {
		return ErrorCode::CIRI_NOT_IMPLEMENTED;
	}

	// generate and bind texture
	glGenTextures(1, &_textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _textureId);

	// set the texture data for each face
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, posx);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, negx);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, posy);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, negy);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, posz);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, negz);

	// unbind texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return ErrorCode::CIRI_OK;
}

void GLTextureCube::destroy() {
	if( _textureId != 0 ) {
		glDeleteTextures(1, &_textureId);
		_textureId = 0;
	}
}

GLuint GLTextureCube::getTextureId() const {
	return _textureId;
}