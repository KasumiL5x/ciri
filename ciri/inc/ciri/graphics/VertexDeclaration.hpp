#ifndef __ciri_graphics_VertexDeclaration__
#define __ciri_graphics_VertexDeclaration__

#include <vector>
#include "VertexElement.hpp"

namespace ciri { namespace graphics {

class VertexDeclaration {
public:
	VertexDeclaration();
	virtual ~VertexDeclaration();

	/**
		* Adds a new VertexElement to the declaration.
		* @param element Element to add.
		*/
	void add( const VertexElement& element );

	/**
		* Gets the total stride of the vertex declaration.
		* @return Stride of all elements within the declaration.
		*/
	int getStride() const;

	/**
		* Gets a vector of all VertexElements that make up this declaration.
		* @return Vector of VertexElements.
		*/
	const std::vector<VertexElement>& getElements() const;

protected:
	std::vector<VertexElement> _elements;
	int _stride;
};

}}

#endif