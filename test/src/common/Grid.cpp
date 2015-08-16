#include "Grid.hpp"

Grid::Grid()
	: _device(nullptr), _initialized(false), _vertexBuffer(nullptr), _shader(nullptr), _constantBuffer(nullptr), _gridSpacing(8.0f),
		_gridExtents(10), _majorSpacing(2), _axisColor(0.0f), _borderColor(0.2f), _gridColor(0.6f), _majorColor(0.5f) {
}

Grid::~Grid() {
}

bool Grid::create( ciri::IGraphicsDevice* device, const std::string& shaderExt ) {
	if( _initialized ) {
		return false;
	}

	_device = device;

	createVertices();

	if( !createBuffers() || !loadShader(shaderExt)) {
		clean();
		return false;
	}

	_initialized = true;

	return true;
}

bool Grid::updateConstants( const cc::Mat4f& xform ) {
	if( !_initialized ) {
		return false;
	}

	_constants.xform = xform;
	return ciri::err::success(_constantBuffer->setData(sizeof(GridConstants), &_constants));
}

void Grid::clean() {
	if( !_initialized ) {
		return;
	}

	_device = nullptr;
	_initialized = false;
	if( _constantBuffer != nullptr ) {
		_constantBuffer->destroy();
	}
	if( _shader != nullptr ) {
		_shader->destroy();
	}
	if( _vertexBuffer != nullptr ) {
		_vertexBuffer->destroy();
	}
}

bool Grid::isValid() const {
	return _initialized;
}

void Grid::setSpacing( float val ) {
	if( _initialized ) {
		return;
	}
	_gridSpacing = val;
}

void Grid::setExtents( int val ) {
	if( _initialized ) {
		return;
	}
	_gridExtents = val;
}

void Grid::setMajorSpacing( int val ) {
	if( _initialized ) {
		return;
	}
	_majorSpacing = val;
}

void Grid::setAxisColor( const cc::Vec3f& val ) {
	if( _initialized ) {
		return;
	}
	_axisColor = val;
}

void Grid::setBorderColor( const cc::Vec3f& val ) {
	if( _initialized ) {
		return;
	}
	_borderColor = val;
}

void Grid::setGridColor( const cc::Vec3f& val ) {
	if( _initialized ) {
		return;
	}
	_gridColor = val;
}

void Grid::setMajorColor( const cc::Vec3f& val ) {
	if( _initialized ) {
		return;
	}
	_majorColor = val;
}


ciri::IVertexBuffer* Grid::getVertexBuffer() const {
	return _vertexBuffer;
}

ciri::IShader* Grid::getShader() const {
	return _shader;
}

void Grid::createVertices() {
	const float ext = static_cast<float>(_gridExtents) * _gridSpacing;
	for( int i = -_gridExtents; i <= _gridExtents; ++i ) {
		const float pos = static_cast<float>(i) * _gridSpacing;
		// line 0
		GridVertex v0;
		v0.position = cc::Vec3f(pos, 0.0f, -ext);
		GridVertex v1;
		v1.position = cc::Vec3f(pos, 0.0f, ext);
		// line 1
		GridVertex v2;
		v2.position = cc::Vec3f(-ext, 0.0f, pos);
		GridVertex v3;
		v3.position = cc::Vec3f(ext, 0.0f, pos);
		// coloring
		if( 0 == i ) {
			v0.color = _axisColor;
			v1.color = _axisColor;
			v2.color = _axisColor;
			v3.color = _axisColor;
		} else if( -_gridExtents == i || _gridExtents == i ) {
			v0.color = _borderColor;
			v1.color = _borderColor;
			v2.color = _borderColor;
			v3.color = _borderColor;
		} else if( abs(i) % _majorSpacing == 0 ) {
			v0.color = _majorColor;
			v1.color = _majorColor;
			v2.color = _majorColor;
			v3.color = _majorColor;
		} else {
			v0.color = _gridColor;
			v1.color = _gridColor;
			v2.color = _gridColor;
			v3.color = _gridColor;
		}

		_gridVertices.push_back(v0);
		_gridVertices.push_back(v1);
		_gridVertices.push_back(v2);
		_gridVertices.push_back(v3);
	}
}

bool Grid::createBuffers() {
	_vertexBuffer = _device->createVertexBuffer();
	return ciri::err::success(_vertexBuffer->set(_gridVertices.data(), sizeof(GridVertex), _gridVertices.size(), false));
}

bool Grid::loadShader( const std::string& shaderExt ) {
	_shader = _device->createShader();
	_shader->addVertexShader(("common/shaders/grid_vs" + shaderExt).c_str());
	_shader->addPixelShader(("common/shaders/grid_ps" + shaderExt).c_str());
	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Position, 0));
	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Color, 0));
	if( ciri::err::failed(_shader->build()) ) {
		printf("Failed to build Grid shader: %s\n", _shader->getLastError());
		return false;
	}

	_constantBuffer = _device->createConstantBuffer();
	if( ciri::err::failed(_constantBuffer->setData(sizeof(GridConstants), &_constants)) ) {
		return false;
	}

	if( ciri::err::failed(_shader->addConstants(_constantBuffer, "GridConstants", ciri::ShaderStage::Vertex)) ) {
		return false;
	}

	return true;
}
