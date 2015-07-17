#include <ciri/gfx/VertexElement.hpp>

namespace ciri {
	VertexElement::VertexElement()
		: _format(VertexFormat::Float3), _usage(VertexUsage::Position), _usageIndex(0) {
	}

	VertexElement::VertexElement( VertexFormat::Type format, VertexUsage::Type usage, int usageIndex )
		: _format(format), _usage(usage), _usageIndex(usageIndex) {
	}

	VertexElement::~VertexElement() {
	}

	VertexFormat::Type VertexElement::getFormat() const {
		return _format;
	}

	VertexUsage::Type VertexElement::getUsage() const {
		return _usage;
	}

	int VertexElement::getUsageIndex() const {
		return _usageIndex;
	}

	int VertexElement::getSize() const {
		switch( _format ) {
			case VertexFormat::Float: {
				return sizeof(float);
			}

			case VertexFormat::Float2: {
				return sizeof(float) * 2;
			}

			case VertexFormat::Float3: {
				return sizeof(float) * 3;
			}

			case VertexFormat::Float4: {
				return sizeof(float) * 4;
			}

			default: {
				return 0;
			}
		}
	}

	int VertexElement::getMultiplicity() const {
		switch( _format ) {
			case VertexFormat::Float: {
				return 1;
			}

			case VertexFormat::Float2: {
				return 2;
			}

			case VertexFormat::Float3: {
				return 3;
			}

			case VertexFormat::Float4: {
				return 4;
			}

			default: {
				return 0;
			}
		}
	}
} // ciri