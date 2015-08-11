#include <ciri/gfx/dx/DXTexture2D.hpp>
#include <ciri/gfx/dx/DXGraphicsDevice.hpp>

namespace ciri {
	DXTexture2D::DXTexture2D( int flags, DXGraphicsDevice* device )
		: ITexture2D(flags), _device(device), _flags(flags), _texture2D(nullptr), _shaderResourceView(nullptr), _width(0), _height(0) {
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

	err::ErrorCode DXTexture2D::setData( int xOffset, int yOffset, int width, int height, void* data, TextureFormat::Type format ) {
		_width = (width > _width) ? width : _width;
		_height = (height > _height) ? height : _height;

		if( _shaderResourceView != nullptr ) {
			// todo: support editing (also change below to dynamic)
			return err::CIRI_NOT_IMPLEMENTED;
		}

		const bool isRenderTarget = (_flags & TextureFlags::RenderTarget);
		const bool mipmaps = (_flags & TextureFlags::Mipmaps);

		D3D11_TEXTURE2D_DESC texDesc;
		ZeroMemory(&texDesc, sizeof(texDesc));
		texDesc.Width = width;
		texDesc.Height = height;
		texDesc.MipLevels = 1; // todo: mipCount;
		texDesc.ArraySize = 1; // todo: arraySize;
		texDesc.Format = ciriToDxFormat(format);
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = getUsage();
		texDesc.BindFlags = getBindFlags();
		texDesc.CPUAccessFlags = getCpuFlags();
		texDesc.MiscFlags = getMiscFlags();

		if( data != nullptr ) {
			D3D11_SUBRESOURCE_DATA subData;
			ZeroMemory(&subData, sizeof(subData));
			subData.pSysMem = data;
			subData.SysMemPitch = width * TextureFormat::bytesPerPixel(format);
			subData.SysMemSlicePitch = width * height * TextureFormat::bytesPerPixel(format);
			if( FAILED(_device->getDevice()->CreateTexture2D(&texDesc, &subData, &_texture2D)) ) {
				destroy();
				return err::CIRI_UNKNOWN_ERROR;
			}
		} else {
			if( FAILED(_device->getDevice()->CreateTexture2D(&texDesc, nullptr, &_texture2D)) ) {
				destroy();
				return err::CIRI_UNKNOWN_ERROR;
			}
		}

		if( FAILED(_device->getDevice()->CreateShaderResourceView(_texture2D, nullptr, &_shaderResourceView)) ) {
			destroy();
			return err::CIRI_UNKNOWN_ERROR;
		}

		if( mipmaps ) {
			_device->getContext()->GenerateMips(_shaderResourceView);
		}

		return err::CIRI_OK;
	}

	int DXTexture2D::getWidth() const {
		return _width;
	}

	int DXTexture2D::getHeight() const {
		return _height;
	}

	ID3D11Texture2D* DXTexture2D::getTexture() const {
		return _texture2D;
	}

	ID3D11ShaderResourceView* DXTexture2D::getShaderResourceView() const {
		return _shaderResourceView;
	}

	DXGI_FORMAT DXTexture2D::ciriToDxFormat( TextureFormat::Type format ) const {
		switch( format ) {
			case TextureFormat::Color: {
				return DXGI_FORMAT_R8G8B8A8_UNORM;
			}

			case TextureFormat::RGB32_Float: {
				return DXGI_FORMAT_R32G32B32_FLOAT; // todo: this breaks DX; do not support it (seems non XYZA formats for textures do), or work around it
			}

			case TextureFormat::RGBA32_Float: {
				return DXGI_FORMAT_R32G32B32A32_FLOAT;
			}

			default: {
				return DXGI_FORMAT_UNKNOWN;
			}
		}
	}

	D3D11_USAGE DXTexture2D::getUsage() const {
		if( _flags & TextureFlags::RenderTarget ) {
			return D3D11_USAGE_DEFAULT;
		}

		if( _flags & TextureFlags::Mipmaps ) {
			return D3D11_USAGE_DEFAULT;
		}

		// todo: if editable, return dynamic

		return D3D11_USAGE_DEFAULT;
	}

	UINT DXTexture2D::getBindFlags() const {
		UINT bindFlags = D3D11_BIND_SHADER_RESOURCE;

		if( _flags & TextureFlags::RenderTarget ) {
			bindFlags |= D3D11_BIND_RENDER_TARGET;
		}

		if( _flags & TextureFlags::Mipmaps ) {
			bindFlags |= D3D11_BIND_RENDER_TARGET; // lol what?
		}

		return bindFlags;
	}

	UINT DXTexture2D::getCpuFlags() const {
		UINT cpuFlags = 0;
		// D3D11_CPU_ACCESS_WRITE ?
		// D3D11_CPU_ACCESS_READ ?
		return cpuFlags;
	}

	UINT DXTexture2D::getMiscFlags() const {
		UINT miscFlags = 0;
		if( _flags & TextureFlags::Mipmaps ) {
			miscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
		}
		return miscFlags;
	}
} // ciri