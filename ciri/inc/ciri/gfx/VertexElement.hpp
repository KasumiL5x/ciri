#ifndef __ciri_vertexelement__
#define __ciri_vertexelement__

#include "VertexFormat.hpp"
#include "VertexUsage.hpp"

namespace ciri {
	class VertexElement {
	public:
		VertexElement();
		VertexElement( int offset, VertexFormat::Type format, VertexUsage::Type usage, int usageIndex );
		~VertexElement();

		int getOffset() const;
		VertexFormat::Type getFormat() const;
		VertexUsage::Type getUsage() const;
		int getUsageIndex() const;
		int getSize() const;
		int getMultiplicity() const;

	private:
		int _offset;
		VertexFormat::Type _format;
		VertexUsage::Type _usage;
		int _usageIndex;
	};
} // ciri

#endif /* __ciri_vertexelement__ */