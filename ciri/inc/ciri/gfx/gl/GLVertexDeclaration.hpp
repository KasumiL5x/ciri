#ifndef __ciri_glvertexdeclaration__
#define __ciri_glvertexdeclaration__

#include "../VertexDeclaration.hpp"

namespace ciri {
	class GLVertexDeclaration : public VertexDeclaration {
	public:
		GLVertexDeclaration();
		virtual ~GLVertexDeclaration();
	};
} // ciri

#endif /* __ciri_glvertexdeclaration__ */