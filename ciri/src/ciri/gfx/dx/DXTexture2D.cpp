#include <ciri/gfx/dx/DXTexture2D.hpp>
#include <ciri/gfx/dx/DXGraphicsDevice.hpp>

namespace ciri {
	DXTexture2D::DXTexture2D( DXGraphicsDevice* device )
		: ITexture2D(), _device(device), _texture2D(nullptr), _shaderResourceView(nullptr) {
	}

	DXTexture2D::~DXTexture2D() {
		destroy();
	}

	void DXTexture2D::destroy() {
		if( _texture2D != nullptr ) {
			_texture2D->Release();
			_texture2D = nullptr;
		}

		if( _shaderResourceView != nullptr ) {
			_shaderResourceView->Release();
			_shaderResourceView = nullptr;
		}
	}

	bool DXTexture2D::setData( int xOffset, int yOffset, int width, int height, void* data, TextureFormat::Type format ) {
		if( _shaderResourceView != nullptr ) {
			// todo: support editing (also change below to dynamic)
			return false;
		}

		D3D11_TEXTURE2D_DESC texDesc;
		ZeroMemory(&texDesc, sizeof(texDesc));
		texDesc.Width = width;
		texDesc.Height = height;
		texDesc.MipLevels = 1; // todo: mipCount;
		texDesc.ArraySize = 1; // todo: arraySize;
		texDesc.Format = ciriToDxFormat(format);
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DYNAMIC; // todo: dynamic if editing
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // todo: write?
		texDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA subData;
		ZeroMemory(&subData, sizeof(subData));
		subData.pSysMem = data;
		subData.SysMemPitch = width * 4; // must be rgba
		subData.SysMemSlicePitch = width*height*4; // must be rgba

		if( FAILED(_device->getDevice()->CreateTexture2D(&texDesc, &subData, &_texture2D)) ) {
			destroy();
			return false;
		}

		if( FAILED(_device->getDevice()->CreateShaderResourceView(_texture2D, nullptr, &_shaderResourceView)) ) {
			destroy();
			return false;
		}	

		return true;
	}

	ID3D11ShaderResourceView* DXTexture2D::getShaderResourceView() const {
		return _shaderResourceView;
	}

	DXGI_FORMAT DXTexture2D::ciriToDxFormat( TextureFormat::Type format ) const {
		switch( format ) {
			case TextureFormat::Color: {
				return DXGI_FORMAT_R8G8B8A8_UNORM;
			}

			default: {
				return DXGI_FORMAT_UNKNOWN;
			}
		}
	}
} // ciri