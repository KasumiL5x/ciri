#include <ciri/gfx/gl/GLTextureCube.hpp>

namespace ciri {
	GLTextureCube::GLTextureCube()
		: ITextureCube() {
	}

	GLTextureCube::~GLTextureCube() {
	}

	err::ErrorCode GLTextureCube::set( int width, int height, void* right, void* left, void* top, void* bottom, void* back, void* front ) {
		// http://antongerdelan.net/opengl/cubemaps.html
		// https://www.opengl.org/wiki/Cubemap_Texture
		// http://learnopengl.com/#!Advanced-OpenGL/Cubemaps
		throw;
	}

	void GLTextureCube::destroy() {
		throw;
	}
} // ciri
