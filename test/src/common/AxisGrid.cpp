#include "AxisGrid.hpp"

AxisGrid::AxisGrid()
	: _device(nullptr), _initialized(false), _vertexBuffer(nullptr), _shader(nullptr), _constantBuffer(nullptr), _gridSpacing(8.0f),
		_gridExtents(10), _majorSpacing(2), _axisColor(0.0f), _borderColor(0.2f), _gridColor(0.6f), _majorColor(0.5f) {
}

AxisGrid::~AxisGrid() {
}

bool AxisGrid::create( std::shared_ptr<ciri::IGraphicsDevice> device ) {
	if( _initialized ) {
		return false;
	}

	_device = device;

	createVertices();

	if( !createBuffers() || !loadShader()) {
		clean();
		return false;
	}

	_initialized = true;

	return true;
}

bool AxisGrid::updateConstants( const cc::Mat4f& xform ) {
	if( !_initialized ) {
		return false;
	}

	_constants.xform = xform;
	return ciri::success(_constantBuffer->setData(sizeof(GridConstants), &_constants));
}

void AxisGrid::clean() {
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

bool AxisGrid::isValid() const {
	return _initialized;
}

void AxisGrid::setSpacing( float val ) {
	if( _initialized ) {
		return;
	}
	_gridSpacing = val;
}

void AxisGrid::setExtents( int val ) {
	if( _initialized ) {
		return;
	}
	_gridExtents = val;
}

void AxisGrid::setMajorSpacing( int val ) {
	if( _initialized ) {
		return;
	}
	_majorSpacing = val;
}

void AxisGrid::setAxisColor( const cc::Vec3f& val ) {
	if( _initialized ) {
		return;
	}
	_axisColor = val;
}

void AxisGrid::setBorderColor( const cc::Vec3f& val ) {
	if( _initialized ) {
		return;
	}
	_borderColor = val;
}

void AxisGrid::setGridColor( const cc::Vec3f& val ) {
	if( _initialized ) {
		return;
	}
	_gridColor = val;
}

void AxisGrid::setMajorColor( const cc::Vec3f& val ) {
	if( _initialized ) {
		return;
	}
	_majorColor = val;
}

const std::shared_ptr<ciri::IVertexBuffer>& AxisGrid::getVertexBuffer() const {
	return _vertexBuffer;
}

const std::shared_ptr<ciri::IShader>& AxisGrid::getShader() const {
	return _shader;
}

void AxisGrid::createVertices() {
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

bool AxisGrid::createBuffers() {
	_vertexBuffer = _device->createVertexBuffer();
	return ciri::success(_vertexBuffer->set(_gridVertices.data(), sizeof(GridVertex), _gridVertices.size(), false));
}

bool AxisGrid::loadShader() {
	_shader = _device->createShader();
	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Position, 0));
	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Color, 0));
		const std::string shaderExt = _device->getShaderExt();
	const std::string vsFile = ("data/shaders/grid_vs" + shaderExt);
	const std::string psFile = ("data/shaders/grid_ps" + shaderExt);
	if( ciri::failed(_shader->loadFromFile(vsFile.c_str(), nullptr, psFile.c_str())) ) {
		printf("Failed to build the Grid shader:\n");
		for( unsigned int i = 0; i < _shader->getErrors().size(); ++i ) {
			printf("%s\n", _shader->getErrors()[i].msg.c_str());
		}
		return false;
	}

	_constantBuffer = _device->createConstantBuffer();
	if( ciri::failed(_constantBuffer->setData(sizeof(GridConstants), &_constants)) ) {
		return false;
	}

	if( ciri::failed(_shader->addConstants(_constantBuffer, "GridConstants", ciri::ShaderStage::Vertex)) ) {
		return false;
	}

	return true;
}
