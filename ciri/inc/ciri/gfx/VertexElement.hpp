#ifndef __ciri_vertexelement__
#define __ciri_vertexelement__

#include "VertexFormat.hpp"
#include "VertexUsage.hpp"

namespace ciri {
	class VertexElement {
	public:
		VertexElement();
		VertexElement( VertexFormat::Format format, VertexUsage::Usage usage, int usageIndex );
		~VertexElement();

		VertexFormat::Format getFormat() const;
		VertexUsage::Usage getUsage() const;
		int getUsageIndex() const;
		int getSize() const;
		int getMultiplicity() const;

	private:
		VertexFormat::Format _format;
		VertexUsage::Usage _usage;
		int _usageIndex;
	};
} // ciri

#endif /* __ciri_vertexelement__ */