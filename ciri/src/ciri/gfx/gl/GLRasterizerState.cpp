#include <gl/glew.h>
#include <ciri/gfx/gl/GLRasterizerState.hpp>

namespace ciri {
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
} // ciri