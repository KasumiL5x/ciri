#include "ShaderPresets.hpp"

SimpleShader::SimpleShader()
	: _shader(nullptr), _constantBuffer(nullptr), _materialConstantsBuffer(nullptr) {
}

SimpleShader::~SimpleShader() {
}

void* SimpleShader::operator new( size_t i ) {
	return _mm_malloc(i, 16); // allocate aligned to 16
}

void SimpleShader::operator delete( void* p ) {
	_mm_free(p);
}

bool SimpleShader::create( std::shared_ptr<ciri::IGraphicsDevice> device ) {
	// create the shader
	_shader = device->createShader();
	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Position, 0));
	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Normal, 0));
	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float4, ciri::VertexUsage::Tangent, 0));
	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float2, ciri::VertexUsage::Texcoord, 0));
	const std::string shaderExt = device->getShaderExt();
	const std::string vsFile = ("common/shaders/simple_vs" + shaderExt);
	const std::string psFile = ("common/shaders/simple_ps" + shaderExt);
	if( ciri::failed(_shader->loadFromFile(vsFile.c_str(), nullptr, psFile.c_str())) ) {
		return false;
	}

	// create constant buffers
	_constantBuffer = device->createConstantBuffer();
	if( ciri::failed(_constantBuffer->setData(sizeof(Constants), &_constants)) ) {
		return false;
	}
	_materialConstantsBuffer = device->createConstantBuffer();
	if( ciri::failed(_materialConstantsBuffer->setData(sizeof(MaterialConstants), &_materialConstants)) ) {
		return false;
	}

	// assign constant buffers
	if( ciri::failed(_shader->addConstants(_constantBuffer, "SimpleConstants", ciri::ShaderStage::Vertex)) ){
		return false;
	}
	if( ciri::failed(_shader->addConstants(_materialConstantsBuffer, "MaterialConstants", ciri::ShaderStage::Pixel)) ) {
		return false;
	}

	return true;
}

const std::shared_ptr<ciri::IShader>& SimpleShader::getShader() const {
	return _shader;
}

SimpleShader::Constants& SimpleShader::getConstants() {
	return _constants;
}

SimpleShader::MaterialConstants& SimpleShader::getMaterialConstants() {
	return _materialConstants;
}

bool SimpleShader::updateConstants() {
	bool success = true;
	if( ciri::failed(_constantBuffer->setData(sizeof(Constants), &_constants)) ) {
		success = false;
	}
	if( ciri::failed(_materialConstantsBuffer->setData(sizeof(MaterialConstants), &_materialConstants)) ) {
		success = false;
	}
	return success;
}