#pragma comment(lib, "D3DCompiler.lib")

#include <ciri/gfx/DXShader.hpp>
#include <ciri/gfx/DXGraphicsDevice.hpp>
#include <d3dcompiler.h>
#include <ciri/util/StrUtil.hpp>

namespace ciri {
	DXShader::DXShader( DXGraphicsDevice* device )
		: IShader(), _device(device), _vsFile(""), _gsFile(""), _psFile(""), _vertexShader(nullptr), _geometryShader(nullptr), _pixelShader(nullptr), _inputLayout(nullptr) {
		_dxUsageStrings[VertexUsage::Position] = "POSITION";
		_dxUsageStrings[VertexUsage::Color] = "COLOR";
		_dxUsageStrings[VertexUsage::Texcoord] = "TEXCOORD";
		_dxUsageStrings[VertexUsage::Normal] = "NORMAL";
		_dxUsageStrings[VertexUsage::Binormal] = "BINORMAL";
		_dxUsageStrings[VertexUsage::Tangent] = "TANGENT";
	}

	DXShader::~DXShader() {
	}

	void DXShader::addVertexShader( const char* filename ) {
		_vsFile = filename;
	}

	void DXShader::addGeometryShader( const char* filename ) {
		_gsFile = filename;
	}

	void DXShader::addPixelShader( const char* filename ) {
		_psFile = filename;
	}

	void DXShader::addInputElement( const VertexElement& element ) {
		_vertexDeclaration.add(element);
	}

	bool DXShader::build() {
		if( _vsFile.empty() && _gsFile.empty() && _psFile.empty() ) {
			return false;
		}

		HRESULT hr = S_OK;

		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
		#if defined(_DEBUG)
			flags |= D3DCOMPILE_DEBUG;
		#endif

		ID3DBlob* shaderBlob = nullptr;
		ID3DBlob* errorBlob = nullptr;

		// build the vertex shader
		if( !_vsFile.empty() ) {
			hr = D3DCompileFromFile(strutil::str2wstr(_vsFile).c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", flags, 0, &shaderBlob, &errorBlob);
			if( FAILED(hr) ) {
				if( HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND) == hr ) {
					_lastError = "File not found";
				} else if( HRESULT_FROM_WIN32(ERROR_PATH_NOT_FOUND) == hr ) {
					_lastError = "Path not found";
				} else {
					_lastError = (const char*)errorBlob->GetBufferPointer();
				}
				destroy();
				return false;
			}

			hr = _device->getDevice()->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &_vertexShader);
			if( FAILED(hr) ) {
				_lastError = (const char*)errorBlob->GetBufferPointer();
				destroy();
				return false;
			}

			// build the input layout
			const std::vector<VertexElement>& elements = _vertexDeclaration.getElements();
			D3D11_INPUT_ELEMENT_DESC* layout = new D3D11_INPUT_ELEMENT_DESC[elements.size()];
			for( unsigned int i = 0; i < elements.size(); ++i ) {
				layout[i].SemanticName = _dxUsageStrings[elements[i].getUsage()].c_str();
				layout[i].SemanticIndex = 0;
				layout[i].Format = convertInputFormat(elements[i].getFormat());
				layout[i].InputSlot = 0;
				layout[i].AlignedByteOffset = elements[i].getOffset();
				layout[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				layout[i].InstanceDataStepRate = 0;
			}
			hr = _device->getDevice()->CreateInputLayout(layout, elements.size(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &_inputLayout);
			if( FAILED(hr) ) {
				_lastError = (const char*)errorBlob->GetBufferPointer();
				destroy();
				return false;
			}

			delete[] layout;
			layout = nullptr;

			shaderBlob->Release();
			if( errorBlob != nullptr ) { errorBlob->Release(); errorBlob = nullptr; }
		}

		// build the geometry shader
		if( !_gsFile.empty() ) {
			hr = D3DCompileFromFile(strutil::str2wstr(_gsFile).c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "gs_5_0", flags, 0, &shaderBlob, &errorBlob);
			if( FAILED(hr) ) {
				if( HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND) == hr ) {
					_lastError = "File not found";
				} else if( HRESULT_FROM_WIN32(ERROR_PATH_NOT_FOUND) == hr ) {
					_lastError = "Path not found";
				} else {
					_lastError = (const char*)errorBlob->GetBufferPointer();
				}
				destroy();
				return false;
			}

			hr = _device->getDevice()->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &_geometryShader);
			if( FAILED(hr) ) {
				_lastError = (const char*)errorBlob->GetBufferPointer();
				destroy();
				return false;
			}

			shaderBlob->Release();
			if( errorBlob != nullptr ) { errorBlob->Release(); errorBlob = nullptr; }
		}

		// build the fragment shader
		if( !_psFile.empty() ) {
			hr = D3DCompileFromFile(strutil::str2wstr(_psFile).c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", flags, 0, &shaderBlob, &errorBlob);
			if( FAILED(hr) ) {
				if( HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND) == hr ) {
					_lastError = "File not found";
				} else if( HRESULT_FROM_WIN32(ERROR_PATH_NOT_FOUND) == hr ) {
					_lastError = "Path not found";
				} else {
					_lastError = (const char*)errorBlob->GetBufferPointer();
				}
				destroy();
				return false;
			}

			hr = _device->getDevice()->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &_pixelShader);
			if( FAILED(hr) ) {
				_lastError = (const char*)errorBlob->GetBufferPointer();
				destroy();
				return false;
			}

			shaderBlob->Release();
			if( errorBlob != nullptr ) { errorBlob->Release(); errorBlob = nullptr; }
		}

		return true;
	}

	bool DXShader::rebuild() {
		destroy();
		return build();
	}

	void DXShader::destroy() {
		if( _inputLayout != nullptr ) {
			_inputLayout->Release();
			_inputLayout = nullptr;
		}

		if( _vertexShader != nullptr ) {
			_vertexShader->Release();
			_vertexShader = nullptr;
		}

		if( _geometryShader != nullptr ) {
			_geometryShader->Release();
			_geometryShader = nullptr;
		}

		if( _pixelShader != nullptr ) {
			_pixelShader->Release();
			_pixelShader = nullptr;
		}
	}

	const char* DXShader::getLastError() const {
		return _lastError.c_str();
	}

	ID3D11VertexShader* DXShader::getVertexShader() const {
		return _vertexShader;
	}

	ID3D11GeometryShader* DXShader::getGeometryShader() const {
		return _geometryShader;
	}

	ID3D11PixelShader* DXShader::getPixelShader() const {
		return _pixelShader;
	}

	ID3D11InputLayout* DXShader::getInputLayout() const {
		return _inputLayout;
	}

	DXGI_FORMAT DXShader::convertInputFormat( VertexFormat::Type type ) const {
		switch( type ) {
			case VertexFormat::Float: {
				return DXGI_FORMAT_R32_FLOAT;
			}

			case VertexFormat::Float2: {
				return DXGI_FORMAT_R32G32_FLOAT;
			}

			case VertexFormat::Float3: {
				return DXGI_FORMAT_R32G32B32_FLOAT;
			}

			case VertexFormat::Float4: {
				return DXGI_FORMAT_R32G32B32A32_FLOAT;
			}

			default: {
				return DXGI_FORMAT_UNKNOWN;
			}
		}
	}
} // ciri