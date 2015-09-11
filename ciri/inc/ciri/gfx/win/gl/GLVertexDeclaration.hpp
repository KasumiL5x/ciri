#ifndef __ciri_gfx_glvertexdeclaration__
#define __ciri_gfx_glvertexdeclaration__

#include <ciri/gfx/VertexDeclaration.hpp>

namespace ciri {
	class GLVertexDeclaration : public VertexDeclaration {
	public:
		GLVertexDeclaration();
		virtual ~GLVertexDeclaration();
	};
} // ciri

#endif /* __ciri_gfx_glvertexdeclaration__ */