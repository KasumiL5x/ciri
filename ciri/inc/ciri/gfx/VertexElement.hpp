#ifndef __ciri_gfx_vertex_element__
#define __ciri_gfx_vertex_element__

#include "VertexFormat.hpp"
#include "VertexUsage.hpp"

namespace ciri {
	class VertexElement {
	public:
		VertexElement();
		VertexElement( VertexFormat::Format format, VertexUsage::Usage usage, int usageIndex );
		~VertexElement();

		/**
		 * Gets the format of the vertex.
		 * @return Format of the vertex as a VertexFormat.
		 */
		VertexFormat::Format getFormat() const;

		/**
		 * Gets the usage of the vertex.
		 * @return Usage of the vertex as a VertexUsage.
		 */
		VertexUsage::Usage getUsage() const;

		/**
		 * Gets the usage index.  For instance, Texture coordinates can have multiple indices (TEXCOORD0, TEXCOORD1, and so on).
		 * @return Usage index.
		 */
		int getUsageIndex() const;

		/**
		 * Gets the size of the vertex element in bytes.
		 * @return Size of the vertex element in bytes.
		 */
		int getSize() const;

		/**
		 * Gets the multiplicity of the vertex element.  For instance, a Float3 has a multiplicity of 3; Float2 of 2, and so on.
		 * @return Vertex element multiplicity.
		 */
		int getMultiplicity() const;

	private:
		VertexFormat::Format _format;
		VertexUsage::Usage _usage;
		int _usageIndex;
	};
} // ciri

#endif /* __ciri_gfx_vertex_element__ */