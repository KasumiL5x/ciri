#include <ciri/gfx/VertexDeclaration.hpp>

namespace ciri {
	VertexDeclaration::VertexDeclaration()
		: _stride(0) {
	}

	VertexDeclaration::~VertexDeclaration() {
	}

	void VertexDeclaration::add( const VertexElement& element ) {
		_stride += element.getSize();
		_elements.push_back(element);
	}

	int VertexDeclaration::getStride() const {
		return _stride;
	}

	const std::vector<VertexElement>& VertexDeclaration::getElements() const {
		return _elements;
	}
} // ciri