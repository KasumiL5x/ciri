#include <ciri/gfx/gl/GLDepthStencilState.hpp>

namespace ciri {
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

	GLenum GLDepthStencilState::ciriToGlOp( const StencilOperation::Operation op ) const {
		switch( op ) {
			case StencilOperation::Keep: {
				return GL_KEEP;
			}
			case StencilOperation::Zero: {
				return GL_ZERO;
			}
			case StencilOperation::Replace: {
				return GL_REPLACE;
			}
			case StencilOperation::Increment: {
				return GL_INCR_WRAP;
			}
			case StencilOperation::Decrement: {
				return GL_DECR_WRAP;
			}
			case StencilOperation::IncrementSaturation: {
				return GL_INCR;
			}
			case StencilOperation::DecrementSaturation: {
				return GL_DECR;
			}
			case StencilOperation::Invert: {
				return GL_INVERT;
			}
			default: {
				return GL_KEEP;
			}
		}
	}
} // ciri