#include "Axis.hpp"
#include <vector>

Axis::Axis()
	: _vertexBuffer(nullptr), _shader(nullptr), _constantsBuffer(nullptr) {
}

Axis::~Axis() {
}

ciri::IVertexBuffer* Axis::getVertexBuffer() const {
	return _vertexBuffer;
}

ciri::IShader* Axis::getShader() const {
	return _shader;
}

bool Axis::isValid() const {
	return (_vertexBuffer != nullptr) && (_shader != nullptr) && (_constantsBuffer != nullptr);
}

bool Axis::create( float length, const std::string& shaderExt, ciri::IGraphicsDevice* device ) {
	std::vector<AxisVertex> vertices;
	const cc::Vec3f RED = cc::Vec3f(1.0f, 0.0f, 0.0f);
	const cc::Vec3f GREEN = cc::Vec3f(0.0f, 1.0f, 0.0f);
	const cc::Vec3f BLUE = cc::Vec3f(0.0f, 0.0f, 1.0f);

	// X
	vertices.push_back(AxisVertex(cc::Vec3f::zero(), RED));
	vertices.push_back(AxisVertex(cc::Vec3f(length, 0.0f, 0.0f), RED));

	// Y
	vertices.push_back(AxisVertex(cc::Vec3f::zero(), GREEN));
	vertices.push_back(AxisVertex(cc::Vec3f(0.0f, length, 0.0f), GREEN));

	//Z
	vertices.push_back(AxisVertex(cc::Vec3f::zero(), BLUE));
	vertices.push_back(AxisVertex(cc::Vec3f(0.0f, 0.0f, length), BLUE));

	_vertexBuffer = device->createVertexBuffer();
	if( ciri::err::failed(_vertexBuffer->set(vertices.data(), sizeof(AxisVertex), 6, false)) ) {
		return false;
	}

	_shader = device->createShader();
	_shader->addVertexShader(("data/axis_vs" + shaderExt).c_str());
	_shader->addPixelShader(("data/axis_ps" + shaderExt).c_str());
	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Position, 0));
	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Color, 0));
	if( ciri::err::failed(_shader->build()) ) {
		return false;
	}

	_constantsBuffer = device->createConstantBuffer();
	if( ciri::err::failed(_constantsBuffer->setData(sizeof(AxisConstants), &_constants)) ) {
		return false;
	}

	if( ciri::err::failed(_shader->addConstants(_constantsBuffer, "AxisConstants", ciri::ShaderStage::Vertex)) ) {
		return false;
	}

	return true;
}

bool Axis::updateConstants( const cc::Mat4f& xform ) {
	if( nullptr == _shader || nullptr == _constantsBuffer ) {
		return false;
	}
	_constants.xform = xform;
	return ciri::err::success(_constantsBuffer->setData(sizeof(AxisConstants), &_constants));
}