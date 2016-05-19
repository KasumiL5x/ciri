#ifndef __ciri_graphics_GLVertexDeclaration__
#define __ciri_graphics_GLVertexDeclaration__

#include <ciri/graphics/VertexDeclaration.hpp>

namespace ciri {

class GLVertexDeclaration : public VertexDeclaration {
public:
	GLVertexDeclaration();
	virtual ~GLVertexDeclaration();
};

}

#endif