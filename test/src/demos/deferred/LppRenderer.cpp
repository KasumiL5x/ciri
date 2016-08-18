#include "LppRenderer.hpp"
#include <cassert>

LppRenderer::LppRenderer()
	: _initialized(false) {
}

LppRenderer::~LppRenderer() {
}

bool LppRenderer::initialize( int width, int height ) {
	assert(!_initialized && width>0 && height>0);

	_width = width;
	_height = height;

	bool success = true;

	if( !createBuffers() ) {
		success = false;
	}

	_initialized = success;
	return success;
}

bool LppRenderer::createBuffers() {
	throw;
}