#ifndef __ciri_graphics_DXShader__
#define __ciri_graphics_DXShader__

#include <memory>
#include <unordered_map>
#include <vector>
#include <string>
#include <d3d11.h>
#include <ciri/graphics/IShader.hpp>
#include <ciri/graphics/VertexDeclaration.hpp>

namespace ciri { namespace graphics {

class DXGraphicsDevice;
class DXConstantBuffer;

class DXShader : public IShader {
public:
	DXShader( const std::shared_ptr<DXGraphicsDevice>& device );
	virtual ~DXShader();

	virtual void addInputElement( const VertexElement& element ) override;
	virtual core::ErrorCode loadFromFile( const char* vs, const char* gs, const char* ps ) override;
	virtual core::ErrorCode loadFromMemory( const char* vs, const char* gs, const char* ps ) override;
	virtual core::ErrorCode addConstants( const std::shared_ptr<IConstantBuffer>& buffer, const char* name, int shaderTypeFlags ) override;
	virtual void destroy() override;
	virtual const std::vector<ShaderError>& getErrors() const override;
	virtual bool isValid() const override;

	ID3D11VertexShader* getVertexShader() const;
	ID3D11GeometryShader* getGeometryShader() const;
	ID3D11PixelShader* getPixelShader() const;
	ID3D11InputLayout* getInputLayout() const;

	const std::vector<std::shared_ptr<DXConstantBuffer>>& getVertexConstants() const;
	const std::vector<std::shared_ptr<DXConstantBuffer>>& getGeometryConstants() const;
	const std::vector<std::shared_ptr<DXConstantBuffer>>& getPixelConstants() const;

private:
	void addError( core::ErrorCode code, const std::string& msg );
	void clearErrors();

private:
	std::shared_ptr<DXGraphicsDevice> _device;
	//
	ID3D11VertexShader* _vertexShader;
	ID3D11GeometryShader* _geometryShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _inputLayout;
	//
	std::vector<ShaderError> _errors;
	//
	VertexDeclaration _vertexDeclaration;
	//
	std::unordered_map<VertexUsage, std::string> _dxUsageStrings;
	//
	std::unordered_map<std::string, int> _vertexConstantBufferIndices;
	std::unordered_map<std::string, int> _geometryConstantBufferIndices;
	std::unordered_map<std::string, int> _pixelConstantBufferIndices;
	std::vector<std::shared_ptr<DXConstantBuffer>> _vertexConstantBuffers;
	std::vector<std::shared_ptr<DXConstantBuffer>> _geometryConstantBuffers;
	std::vector<std::shared_ptr<DXConstantBuffer>> _pixelConstantBuffers;
};

}}

#endif