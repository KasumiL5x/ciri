#include <ciri/gfx/win/dx/DXTexture2D.hpp>
#include <ciri/gfx/win/dx/DXGraphicsDevice.hpp>
#include <ciri/gfx/win/dx/CiriToDx.hpp>
#include <ciri/gfx/win/dx/msft/ScreenGrab.h>
#include <ciri/core/StrUtil.hpp>

namespace ciri {
	DXTexture2D::DXTexture2D( int flags, const std::shared_ptr<DXGraphicsDevice>& device )
		: ITexture2D(flags), _device(device), _flags(flags), _format(TextureFormat::Color), _texture2D(nullptr), _shaderResourceView(nullptr), _width(0), _height(0) {
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

	ErrorCode DXTexture2D::setData( int xOffset, int yOffset, int width, int height, void* data, TextureFormat::Format format ) {
		// update if already valid
		if( _shaderResourceView != nullptr ) {
			// format must be the same
			if( format != _format ) {
				return ErrorCode::CIRI_INVALID_ARGUMENT;
			}

			// for now, the enture texture must be updated, and it must therefore be of the same size
			if( width != _width || height != _height || xOffset != 0 || yOffset != 0 ) {
				return ErrorCode::CIRI_NOT_IMPLEMENTED;
			}

			// todo: support editing (also change below to dynamic)
			return ErrorCode::CIRI_NOT_IMPLEMENTED;
		}

		// offsets must be zero when initializing the texture
		if( xOffset != 0 || yOffset != 0 ) {
			return ErrorCode::CIRI_INVALID_ARGUMENT;
		}

		// width and height must be positive
		if( width <= 0 || height <= 0 ) {
			return ErrorCode::CIRI_INVALID_ARGUMENT;
		}

		_width = width;
		_height = height;
		_format = format;

		D3D11_TEXTURE2D_DESC texDesc;
		ZeroMemory(&texDesc, sizeof(texDesc));
		texDesc.Width = width;
		texDesc.Height = height;
		texDesc.MipLevels = getMipLevels();
		texDesc.ArraySize = 1; // todo: arraySize (for cube textures and whatnot)
		texDesc.Format = ciriToDxTextureFormat(format);
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = getUsage();
		texDesc.BindFlags = getBindFlags();
		texDesc.CPUAccessFlags = getCpuFlags();
		texDesc.MiscFlags = getMiscFlags();

		// multisampled textures require initialization with nullptr first, so just do it this way for everything for simplicity
		if( FAILED(_device->getDevice()->CreateTexture2D(&texDesc, nullptr, &_texture2D)) ) {
			destroy();
			return ErrorCode::CIRI_UNKNOWN_ERROR; // todo: texture create failure
		}

		// update the subresource (a.k.a set pixel data) if there is any
		if( data != nullptr ) {
			const UINT pitch = width * TextureFormat::bytesPerPixel(format);
			_device->getContext()->UpdateSubresource(_texture2D, 0, nullptr, data, pitch, 0);
		}

		// create actual shader resource view
		if( FAILED(_device->getDevice()->CreateShaderResourceView(_texture2D, nullptr, &_shaderResourceView)) ) {
			destroy();
			return ErrorCode::CIRI_UNKNOWN_ERROR; // todo: texture create failure
		}

		// generate mipmaps
		if( _flags & TextureFlags::Mipmaps ) {
			_device->getContext()->GenerateMips(_shaderResourceView);
		}

		return ErrorCode::CIRI_OK;
	}

	int DXTexture2D::getWidth() const {
		return _width;
	}

	int DXTexture2D::getHeight() const {
		return _height;
	}

	ErrorCode DXTexture2D::writeToTGA( const char* file ) {
		// todo: somehow figure this out!
		// https://github.com/Microsoft/DirectXTK/blob/master/Src/ScreenGrab.cpp
		// http://www.gamedev.net/topic/647297-id3d11shaderresourceview-save-to-file/
		// 1. create stage texture (cpu=read, usage=staging)
		// 2. copy resource (context->CopyResource(stage, source)
		// 3. map to get pixels (d3d11_mapped_subresource; context->map(stage, 0, read, 0, map)...)
		// 4. somehow convert this into TGA
		// 5. save as TGA
		return ErrorCode::CIRI_NOT_IMPLEMENTED;
	}

	ErrorCode DXTexture2D::writeToDDS( const char* file ) {
		// todo: return ciri error codes instead of a boolean
		if( nullptr == file || nullptr == _texture2D ) {
			return ErrorCode::CIRI_UNKNOWN_ERROR; // todo
		}
		if( FAILED(DirectX::SaveDDSTextureToFile(_device->getContext(), _texture2D, strutil::str2wstr_utf8(file).c_str())) ) {
			return ErrorCode::CIRI_UNKNOWN_ERROR; // todo
		}
		return ErrorCode::CIRI_OK;
	}

	ID3D11Texture2D* DXTexture2D::getTexture() const {
		return _texture2D;
	}

	ID3D11ShaderResourceView* DXTexture2D::getShaderResourceView() const {
		return _shaderResourceView;
	}

	UINT DXTexture2D::getMipLevels() const {
		return (_flags & TextureFlags::Mipmaps) ? 0 : 1;
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