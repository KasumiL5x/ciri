#include <ciri/graphics/win/gl/GLRasterizerState.hpp>

using namespace ciri::graphics;

GLRasterizerState::GLRasterizerState()
	: IRasterizerState() {
}

GLRasterizerState::~GLRasterizerState() {
	destroy();
}

bool GLRasterizerState::create( const RasterizerDesc& desc ) {
	_desc = desc;
	return true;
}

void GLRasterizerState::destroy() {
	// nothing :D
}

const RasterizerDesc& GLRasterizerState::getDesc() const {
	return _desc;
}