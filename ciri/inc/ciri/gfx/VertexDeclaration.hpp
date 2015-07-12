#ifndef __ciri_vertexdeclaration__
#define __ciri_vertexdeclaration__

#include <vector>
#include "VertexElement.hpp"

namespace ciri {
	class VertexDeclaration {
	public:
		VertexDeclaration();
		virtual ~VertexDeclaration();

		void add( const VertexElement& element );
		int getStride() const;
		const std::vector<VertexElement>& getElements() const;

	protected:
		std::vector<VertexElement> _elements;
		int _stride;
	};
} // ciri

#endif /* __ciri_vertexdeclaration__ */