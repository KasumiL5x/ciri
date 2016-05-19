#include <ciri/graphics/win/gl/GLBlendState.hpp>

using namespace ciri::graphics;

GLBlendState::GLBlendState()
	: IBlendState() {
}

GLBlendState::~GLBlendState() {
	destroy();
}

bool GLBlendState::create( const BlendDesc& desc ) {
	_desc = desc;
	return true;
}

void GLBlendState::destroy() {
}

const BlendDesc& GLBlendState::getDesc() const {
	return _desc;
}