#include <ciri/graphics/win/gl/GLBlendState.hpp>

using namespace ciri;

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