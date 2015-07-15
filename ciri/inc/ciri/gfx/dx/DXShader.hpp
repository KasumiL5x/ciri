#ifndef __ciri_dxshader__
#define __ciri_dxshader__

#include <unordered_map>
#include <vector>
#include <string>
#include <d3d11_1.h>
#include "../IShader.hpp"
#include "../VertexDeclaration.hpp"

namespace ciri {
	class DXGraphicsDevice;

	class DXShader : public IShader {
	public:
		DXShader( DXGraphicsDevice* device );
		virtual ~DXShader();

		virtual void addVertexShader( const char* filename );
		virtual void addGeometryShader( const char* filename );
		virtual void addPixelShader( const char* filename );
		virtual void addInputElement( const VertexElement& element );
		virtual bool build();
		virtual bool rebuild();
		virtual void destroy();
		virtual const char* getLastError() const;
		virtual bool isValid() const;

		ID3D11VertexShader* getVertexShader() const;
		ID3D11GeometryShader* getGeometryShader() const;
		ID3D11PixelShader* getPixelShader() const;
		ID3D11InputLayout* getInputLayout() const;

	private:
		DXGI_FORMAT convertInputFormat( VertexFormat::Type type ) const;

	private:
		DXGraphicsDevice* _device;
		//
		std::string _vsFile;
		std::string _gsFile;
		std::string _psFile;
		//
		ID3D11VertexShader* _vertexShader;
		ID3D11GeometryShader* _geometryShader;
		ID3D11PixelShader* _pixelShader;
		ID3D11InputLayout* _inputLayout;
		//
		VertexDeclaration _vertexDeclaration;
		std::string _lastError;
		//
		std::unordered_map<VertexUsage::Type, std::string> _dxUsageStrings;
	};
} // ciri

#endif /* __ciri_dxshader__ */