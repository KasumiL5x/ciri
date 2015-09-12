#include <ciri/gfx/win/dx/DXShader.hpp>
#include <ciri/gfx/win/dx/DXGraphicsDevice.hpp>
#include <ciri/gfx/win/dx/CiriToDx.hpp>
#include <ciri/core/File.hpp>
#include <d3dcompiler.h>

namespace ciri {
	DXShader::DXShader( const std::shared_ptr<DXGraphicsDevice>& device )
		: IShader(), _device(device), _vertexShader(nullptr), _geometryShader(nullptr), _pixelShader(nullptr), _inputLayout(nullptr) {
		_dxUsageStrings[VertexUsage::Position] = "POSITION";
		_dxUsageStrings[VertexUsage::Color] = "COLOR";
		_dxUsageStrings[VertexUsage::Texcoord] = "TEXCOORD";
		_dxUsageStrings[VertexUsage::Normal] = "NORMAL";
		_dxUsageStrings[VertexUsage::Binormal] = "BINORMAL";
		_dxUsageStrings[VertexUsage::Tangent] = "TANGENT";
	}

	DXShader::~DXShader() {
	}

	void DXShader::addInputElement( const VertexElement& element ) {
		_vertexDeclaration.add(element);
	}

	ErrorCode DXShader::addConstants( IConstantBuffer* buffer, const char* name, int shaderTypeFlags ) {
		if( nullptr == buffer || nullptr == name ) {
			return ErrorCode::CIRI_INVALID_ARGUMENT;
		}

		if( !isValid() ) {
			return ErrorCode::CIRI_SHADER_INVALID;
		}

		DXConstantBuffer* dxBuffer = reinterpret_cast<DXConstantBuffer*>(buffer);

		bool wasBufferSet = false;

		const bool all = (shaderTypeFlags & ShaderStage::All) != 0;

		if( all || (shaderTypeFlags & ShaderStage::Vertex) ) {
			const std::unordered_map<std::string, int>::const_iterator existing = _vertexConstantBufferIndices.find(name);
			if( _vertexConstantBufferIndices.end() == existing ) {
				return ErrorCode::CIRI_UNKNOWN_ERROR; // no matching string
			}
			dxBuffer->setIndex(existing->second);
			_vertexConstantBuffers.push_back(dxBuffer);
			
			wasBufferSet = true;
		}

		if( _geometryShader != nullptr && (all || (shaderTypeFlags & ShaderStage::Geometry)) ) {
			const std::unordered_map<std::string, int>::const_iterator existing = _geometryConstantBufferIndices.find(name);
			if( _geometryConstantBufferIndices.end() == existing ) {
				return ErrorCode::CIRI_UNKNOWN_ERROR;
			}
			dxBuffer->setIndex(existing->second);
			_geometryConstantBuffers.push_back(dxBuffer);

			wasBufferSet = true;
		}

		if( all || (shaderTypeFlags & ShaderStage::Pixel) ) {
			const std::unordered_map<std::string, int>::const_iterator existing = _pixelConstantBufferIndices.find(name);
			if( _pixelConstantBufferIndices.end() == existing ) {
				return ErrorCode::CIRI_UNKNOWN_ERROR;
			}
			dxBuffer->setIndex(existing->second);
			_pixelConstantBuffers.push_back(dxBuffer);

			wasBufferSet = true;
		}

		// if no buffer was set, the shader type flag didn't trigger any of the above conditions
		if( false == wasBufferSet ) {
			return ErrorCode::CIRI_INVALID_ARGUMENT;
		}

		return ErrorCode::CIRI_OK;
	}

	ErrorCode DXShader::loadFromFile( const char* vs, const char* gs, const char* ps ) {
		clearErrors();

		// must have at least VS and PS
		if( nullptr == vs || nullptr == ps ) {
			addError(ErrorCode::CIRI_SHADER_INCOMPLETE, getErrorString(ErrorCode::CIRI_SHADER_INCOMPLETE));
			return ErrorCode::CIRI_SHADER_INCOMPLETE;
		}

		// load vs file
		File vsFile(vs, File::Flags::ReadOnly);
		if( !vsFile.isOpen() ) {
			addError(ErrorCode::CIRI_FILE_NOT_FOUND, getErrorString(ErrorCode::CIRI_FILE_NOT_FOUND) + std::string(" (") + vs + std::string(")"));
			return ErrorCode::CIRI_FILE_NOT_FOUND;
		}
		const std::string vsStr = vsFile.toString();

		// load gs file
		std::string gsStr = ""; // optional shader, so create empty string for it now
		if( gs != nullptr ) {
			File gsFile(gs, File::Flags::ReadOnly);
			if( !gsFile.isOpen() ) {
				addError(ErrorCode::CIRI_FILE_NOT_FOUND, getErrorString(ErrorCode::CIRI_FILE_NOT_FOUND) + std::string(" (") + gs + std::string(")"));
				return ErrorCode::CIRI_FILE_NOT_FOUND;
			}
			gsStr = gsFile.toString();
		}

		// load ps file
		File psFile(ps, File::Flags::ReadOnly);
		if( !psFile.isOpen() ) {
			addError(ErrorCode::CIRI_FILE_NOT_FOUND, getErrorString(ErrorCode::CIRI_FILE_NOT_FOUND) + std::string(" (") + ps + std::string(")"));
			return ErrorCode::CIRI_FILE_NOT_FOUND;
		}
		const std::string psStr = psFile.toString();

		return loadFromMemory(vsStr.c_str(), (nullptr==gs) ? nullptr : gsStr.c_str(), psStr.c_str());
	}

	ErrorCode DXShader::loadFromMemory( const char* vs, const char* gs, const char* ps ) {
		// todo: if valid, destroy and make new one

		clearErrors();

		// must have at least VS and PS
		if( nullptr == vs || nullptr == ps ) {
			addError(ErrorCode::CIRI_SHADER_INCOMPLETE, getErrorString(ErrorCode::CIRI_SHADER_INCOMPLETE));
			return ErrorCode::CIRI_SHADER_INCOMPLETE;
		}

		HRESULT hr = S_OK;

		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
		#if defined(_DEBUG)
			flags |= D3DCOMPILE_DEBUG;
		#endif

		ID3DBlob* shaderBlob = nullptr;
		ID3DBlob* errorBlob = nullptr;

		// todo: change all unknown errors below to proper error codes

		// build the vertex shader
		{
			hr = D3DCompile(vs, strlen(vs), NULL, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", flags, 0, &shaderBlob, &errorBlob);
			if( FAILED(hr) ) {
				addError(ErrorCode::CIRI_SHADER_COMPILE_FAILED, getErrorString(ErrorCode::CIRI_SHADER_COMPILE_FAILED) + std::string(": ") + std::string((const char*)errorBlob->GetBufferPointer()));
			} else {
				hr = _device->getDevice()->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &_vertexShader);
				if( FAILED(hr) ) {
					addError(ErrorCode::CIRI_UNKNOWN_ERROR, getErrorString(ErrorCode::CIRI_UNKNOWN_ERROR) + std::string(": ") + std::string((const char*)errorBlob->GetBufferPointer()));
				} else {
					// build the input layout
					int offset = 0;
					const std::vector<VertexElement>& elements = _vertexDeclaration.getElements();
					D3D11_INPUT_ELEMENT_DESC* layout = new D3D11_INPUT_ELEMENT_DESC[elements.size()];
					for( unsigned int i = 0; i < elements.size(); ++i ) {
						layout[i].SemanticName = _dxUsageStrings[elements[i].getUsage()].c_str();
						layout[i].SemanticIndex = elements[i].getUsageIndex();
						layout[i].Format = ciriToDxVertexFormat(elements[i].getFormat());
						layout[i].InputSlot = 0;
						layout[i].AlignedByteOffset = offset;
						layout[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
						layout[i].InstanceDataStepRate = 0;
						offset += elements[i].getSize(); // sizeof(datatype) * numberOfThem;
					}
					hr = _device->getDevice()->CreateInputLayout(layout, elements.size(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &_inputLayout);
					delete[] layout; layout = nullptr;
					if( FAILED(hr) ) {
						addError(ErrorCode::CIRI_UNKNOWN_ERROR, getErrorString(ErrorCode::CIRI_UNKNOWN_ERROR) + std::string(": ") + std::string((const char*)errorBlob->GetBufferPointer()));
					} else {
						// reflect the constant buffers of the vertex shader
						ID3D11ShaderReflection* refl = nullptr;
						D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&refl);
						int index = 0;
						while( true ) {
							D3D11_SHADER_BUFFER_DESC desc;
							if( FAILED(refl->GetConstantBufferByIndex(index)->GetDesc(&desc)) ) {
								break;
							}

							_vertexConstantBufferIndices[std::string(desc.Name)] = index;

							index += 1;
						}
					} // CreateInputLayout success
				} // CreateVertexShader success
			} // D3DCompile success

			// release shader and error blobs
			if( shaderBlob != nullptr ) { shaderBlob->Release(); shaderBlob = nullptr; }
			if( errorBlob != nullptr ) { errorBlob->Release(); errorBlob = nullptr; }
		}

		// build the geometry shader
		if( gs != nullptr ) {
			hr = D3DCompile(gs, strlen(gs), NULL, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "gs_5_0", flags, 0, &shaderBlob, &errorBlob);
			if( FAILED(hr) ) {
				addError(ErrorCode::CIRI_SHADER_COMPILE_FAILED, getErrorString(ErrorCode::CIRI_SHADER_COMPILE_FAILED) + std::string(": ") + std::string((const char*)errorBlob->GetBufferPointer()));
			} else {
				hr = _device->getDevice()->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &_geometryShader);
				if( FAILED(hr) ) {
					addError(ErrorCode::CIRI_UNKNOWN_ERROR, getErrorString(ErrorCode::CIRI_UNKNOWN_ERROR) + std::string(": ") + std::string((const char*)errorBlob->GetBufferPointer()));
				} else {
					// reflect the constant buffers of the geometry shader
					ID3D11ShaderReflection* refl = nullptr;
					D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&refl);
					int index = 0;
					while( true ) {
						D3D11_SHADER_BUFFER_DESC desc;
						if( FAILED(refl->GetConstantBufferByIndex(index)->GetDesc(&desc)) ) {
							break;
						}
						_geometryConstantBufferIndices[std::string(desc.Name)] = index;

						index += 1;
					}
				} // CreateGeometryShader success
			} // D3DCompile success

			// release shader and error blobs
			if( shaderBlob != nullptr ) { shaderBlob->Release(); shaderBlob = nullptr; }
			if( errorBlob != nullptr ) { errorBlob->Release(); errorBlob = nullptr; }
		}

		// build the pixel shader
		{
			hr = D3DCompile(ps, strlen(ps), NULL, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", flags, 0, &shaderBlob, &errorBlob);
			if( FAILED(hr) ) {
				addError(ErrorCode::CIRI_SHADER_COMPILE_FAILED, getErrorString(ErrorCode::CIRI_SHADER_COMPILE_FAILED) + std::string(": ") + std::string((const char*)errorBlob->GetBufferPointer()));
			} else {
				hr = _device->getDevice()->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &_pixelShader);
				if( FAILED(hr) ) {
					addError(ErrorCode::CIRI_UNKNOWN_ERROR, getErrorString(ErrorCode::CIRI_UNKNOWN_ERROR) + std::string(": ") + std::string((const char*)errorBlob->GetBufferPointer()));
				} else {
					// reflect the constant buffers of the pixel shader
					ID3D11ShaderReflection* refl = nullptr;
					D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&refl);
					int index = 0;
					while( true ) {
						D3D11_SHADER_BUFFER_DESC desc;
						if( FAILED(refl->GetConstantBufferByIndex(index)->GetDesc(&desc)) ) {
							break;
						}
						_pixelConstantBufferIndices[std::string(desc.Name)] = index;

						index += 1;
					}
				} // CreatePixelShader success
			} // D3DCompile success

			// release shader and error blobs
			if( shaderBlob != nullptr ) { shaderBlob->Release(); shaderBlob = nullptr; }
			if( errorBlob != nullptr ) { errorBlob->Release(); errorBlob = nullptr; }
		}

		// if any errors have occurred, clean up and return the first error code
		if( !_errors.empty() ) {
			destroy();
			return _errors[0].code;
		}

		return ErrorCode::CIRI_OK;
	}

	void DXShader::destroy() {
		_vertexConstantBufferIndices.clear();
		_geometryConstantBufferIndices.clear();
		_pixelConstantBufferIndices.clear();
		_vertexConstantBuffers.clear();
		_geometryConstantBuffers.clear();
		_pixelConstantBuffers.clear();


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

	const std::vector<IShader::ShaderError>& DXShader::getErrors() const {
		return _errors;
	}

	bool DXShader::isValid() const {
		return (_vertexShader != nullptr) && (_pixelShader != nullptr);
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

	const std::vector<DXConstantBuffer*>& DXShader::getVertexConstants() const {
		return _vertexConstantBuffers;
	}

	const std::vector<DXConstantBuffer*>& DXShader::getGeometryConstants() const {
		return _geometryConstantBuffers;
	}

	const std::vector<DXConstantBuffer*>& DXShader::getPixelConstants() const {
		return _pixelConstantBuffers;
	}

	void DXShader::addError( ErrorCode code, const std::string& msg ) {
		_errors.push_back(ShaderError(code, msg));
	}

	void DXShader::clearErrors() {
		_errors.clear();
	}
} // ciri