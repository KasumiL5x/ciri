#ifndef __ciri_graphics_DXVertexDeclaration__
#define __ciri_graphics_DXVertexDeclaration__

#include <ciri/graphics/VertexDeclaration.hpp>

namespace ciri { namespace graphics {

class DXVertexDeclaration : public VertexDeclaration {
public:
	DXVertexDeclaration();
	virtual ~DXVertexDeclaration();
};

}}

#endif