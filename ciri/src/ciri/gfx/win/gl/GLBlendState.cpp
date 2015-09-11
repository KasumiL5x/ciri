#include <gl/glew.h>
#include <ciri/gfx/win/gl/GLBlendState.hpp>

namespace ciri {
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
} // ciri
