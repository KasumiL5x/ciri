#ifndef __ciri_graphics_GLVertexDeclaration__
#define __ciri_graphics_GLVertexDeclaration__

#include <ciri/graphics/VertexDeclaration.hpp>

namespace ciri { namespace graphics {

class GLVertexDeclaration : public VertexDeclaration {
public:
	GLVertexDeclaration();
	virtual ~GLVertexDeclaration();
};

}}

#endif