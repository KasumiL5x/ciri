#include <ciri/graphics/win/gl/GLDepthStencilState.hpp>

using namespace ciri;

GLDepthStencilState::GLDepthStencilState()
	: IDepthStencilState() {
}

GLDepthStencilState::~GLDepthStencilState() {
	destroy();
}

bool GLDepthStencilState::create( const DepthStencilDesc& desc ) {
	_desc = desc;
	return true;
}

void GLDepthStencilState::destroy() {
}

const DepthStencilDesc& GLDepthStencilState::getDesc() const {
	return _desc;
}