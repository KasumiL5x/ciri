#include "ShaderPresets.hpp"

namespace core = ciri::core;
namespace gfx = ciri::graphics;

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

bool SimpleShader::create( std::shared_ptr<gfx::IGraphicsDevice> device ) {
	// create the shader
	_shader = device->createShader();
	_shader->addInputElement(gfx::VertexElement(gfx::VertexFormat::Float3, gfx::VertexUsage::Position, 0));
	_shader->addInputElement(gfx::VertexElement(gfx::VertexFormat::Float3, gfx::VertexUsage::Normal, 0));
	_shader->addInputElement(gfx::VertexElement(gfx::VertexFormat::Float4, gfx::VertexUsage::Tangent, 0));
	_shader->addInputElement(gfx::VertexElement(gfx::VertexFormat::Float2, gfx::VertexUsage::Texcoord, 0));
	const std::string shaderExt = device->getShaderExt();
	const std::string vsFile = ("common/shaders/simple_vs" + shaderExt);
	const std::string psFile = ("common/shaders/simple_ps" + shaderExt);
	if( core::failed(_shader->loadFromFile(vsFile.c_str(), nullptr, psFile.c_str())) ) {
		return false;
	}

	// create constant buffers
	_constantBuffer = device->createConstantBuffer();
	if( core::failed(_constantBuffer->setData(sizeof(Constants), &_constants)) ) {
		return false;
	}
	_materialConstantsBuffer = device->createConstantBuffer();
	if( core::failed(_materialConstantsBuffer->setData(sizeof(MaterialConstants), &_materialConstants)) ) {
		return false;
	}

	// assign constant buffers
	if( core::failed(_shader->addConstants(_constantBuffer, "SimpleConstants", gfx::ShaderStage::Vertex)) ){
		return false;
	}
	if( core::failed(_shader->addConstants(_materialConstantsBuffer, "MaterialConstants", gfx::ShaderStage::Pixel)) ) {
		return false;
	}

	return true;
}

const std::shared_ptr<gfx::IShader>& SimpleShader::getShader() const {
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
	if( core::failed(_constantBuffer->setData(sizeof(Constants), &_constants)) ) {
		success = false;
	}
	if( core::failed(_materialConstantsBuffer->setData(sizeof(MaterialConstants), &_materialConstants)) ) {
		success = false;
	}
	return success;
}