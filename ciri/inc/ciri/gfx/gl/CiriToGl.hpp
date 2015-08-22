#ifndef __ciri_ciritogl__
#define __ciri_ciritogl__

#include <gl/glew.h>
#include "../CompareFunction.hpp"

namespace ciri {
	static GLenum ciriToGlFunc( CompareFunction::Function func ) {
		switch( func ) {
			case CompareFunction::Always: {
				return GL_ALWAYS;
			}
			case CompareFunction::Never: {
				return GL_NEVER;
			}
			case CompareFunction::Less: {
				return GL_LESS;
			}
			case CompareFunction::LessEqual: {
				return GL_LEQUAL;
			}
			case CompareFunction::Equal: {
				return GL_EQUAL;
			}
			case CompareFunction::GreaterEqual: {
				return GL_GEQUAL;
			}
			case CompareFunction::Greater: {
				return GL_GREATER;
			}
			case CompareFunction::NotEqual: {
				return GL_NOTEQUAL;
			}
			default: {
					return GL_NEVER;
			}
		}
	}
} // ciri

#endif /* __ciri_ciritogl__ */