#include <ciri/gfx/dx/DXTexture2D.hpp>
#include <ciri/gfx/dx/DXGraphicsDevice.hpp>
#include <ciri/gfx/dx/msft/ScreenGrab.h>
#include <ciri/util/StrUtil.hpp>

namespace ciri {
	DXTexture2D::DXTexture2D( int flags, DXGraphicsDevice* device )
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

	err::ErrorCode DXTexture2D::setData( int xOffset, int yOffset, int width, int height, void* data, TextureFormat::Type format ) {
		// todo: some check about size differences when updating

		_width = (width > _width) ? width : _width;
		_height = (height > _height) ? height : _height;

		if( _shaderResourceView != nullptr ) {
			// todo: support editing (also change below to dynamic)
			return err::CIRI_NOT_IMPLEMENTED;
		}

		_format = format;

		D3D11_TEXTURE2D_DESC texDesc;
		ZeroMemory(&texDesc, sizeof(texDesc));
		texDesc.Width = width;
		texDesc.Height = height;
		texDesc.MipLevels = getMipLevels();
		texDesc.ArraySize = 1; // todo: arraySize (for cube textures and whatnot)
		texDesc.Format = ciriToDxFormat(format);
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = getUsage();
		texDesc.BindFlags = getBindFlags();
		texDesc.CPUAccessFlags = getCpuFlags();
		texDesc.MiscFlags = getMiscFlags();

		// multisampled textures require initialization with nullptr first, so just do it this way for everything for simplicity
		if( FAILED(_device->getDevice()->CreateTexture2D(&texDesc, nullptr, &_texture2D)) ) {
			destroy();
			return err::CIRI_UNKNOWN_ERROR;
		}

		// update the subresource (a.k.a set pixel data) if there is any
		if( data != nullptr ) {
			const UINT pitch = width * TextureFormat::bytesPerPixel(format);
			_device->getContext()->UpdateSubresource(_texture2D, 0, nullptr, data, pitch, 0);
		}

		if( FAILED(_device->getDevice()->CreateShaderResourceView(_texture2D, nullptr, &_shaderResourceView)) ) {
			destroy();
			return err::CIRI_UNKNOWN_ERROR;
		}

		if( _flags & TextureFlags::Mipmaps ) {
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

	bool DXTexture2D::writeToTGA( const char* file ) {
		// todo: somehow figure this out!
		// https://github.com/Microsoft/DirectXTK/blob/master/Src/ScreenGrab.cpp
		// http://www.gamedev.net/topic/647297-id3d11shaderresourceview-save-to-file/
		// 1. create stage texture (cpu=read, usage=staging)
		// 2. copy resource (context->CopyResource(stage, source)
		// 3. map to get pixels (d3d11_mapped_subresource; context->map(stage, 0, read, 0, map)...)
		// 4. somehow convert this into TGA
		// 5. save as TGA

		return false;

		//DirectX::ScratchImage scratchImage;
		//DirectX::CaptureTexture(_device->getDevice(), _device->getContext(), _texture2D, scratchImage);
		//HRESULT hr = (DirectX::SaveToDDSFile(*scratchImage.GetImage(0, 0, 0), 0, (LPCWSTR)file));
		//return SUCCEEDED(hr);

		// todo: none of this works because of differing datatypes...fixme
		// https://github.com/Microsoft/DirectXTK/blob/master/Src/ScreenGrab.cpp
		// http://www.gamedev.net/topic/647297-id3d11shaderresourceview-save-to-file/
		//njhriehtueirwjeirhurgejeiruhi

		//D3D11_TEXTURE2D_DESC desc;
		//ZeroMemory(&desc, sizeof(desc));
		//desc.Width = _width;
		//desc.Height = _height;
		//desc.MipLevels = 1;
		//desc.ArraySize = 1;
		//desc.Format = ciriToDxFormat(_format);
		//desc.BindFlags = 0;
		//desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		//desc.SampleDesc.Count = 1;
		//desc.SampleDesc.Quality = 0;
		//desc.Usage = D3D11_USAGE_STAGING;
		//desc.MiscFlags = 0;
		//ID3D11Texture2D* stageTex = nullptr;
		//if( FAILED(_device->getDevice()->CreateTexture2D(&desc, nullptr, &stageTex)) ) {
		//	return false;
		//}
		//_device->getContext()->CopyResource(stageTex, _texture2D);

		//D3D11_MAPPED_SUBRESOURCE map;
		//if( FAILED(_device->getContext()->Map(stageTex, 0, D3D11_MAP_READ, 0, &map)) ) {
		//	stageTex->Release();
		//	return false;
		//}
		//FLOAT* sourcePixels = new FLOAT[_width * _height * 4 * sizeof(FLOAT)];
		//memcpy(sourcePixels, map.pData, _width * _height * 4);
		//_device->getContext()->Unmap(stageTex, 0);
		//stageTex->Release();
		//unsigned char* destPixels = new unsigned char[_width * _height * 4];
		//unsigned int index = 0;
		//for( UINT row = 0; row < _height; ++row ) {
		//	const UINT rowStart = row * map.RowPitch / 4;
		//	for( UINT col = 0; col < _width; ++col ) {
		//		destPixels[index]   = sourcePixels[rowStart + col * 4 + 0] * 255.0f;
		//		destPixels[index+1] = sourcePixels[rowStart + col * 4 + 1] * 255.0f;
		//		destPixels[index+2] = sourcePixels[rowStart + col * 4 + 2] * 255.0f;
		//		destPixels[index+3] = 255;
		//		index += 4;
		//	}
		//}
		//bool success = TGA::writeToFile(file, _width, _height, destPixels, TGA::RGBA, false);
		//delete[] destPixels;
		//delete[] sourcePixels;
		//return success;

		//auto gpuPtr = reinterpret_cast<const unsigned char*>(map.pData);
		//const int gpuSize = _height * map.RowPitch;
		//unsigned char* sourcePixels = new unsigned char[gpuSize];
		//memcpy(sourcePixels, gpuPtr, gpuSize);
		//_device->getContext()->Unmap(stageTex, 0);

		//unsigned char* destPixels = new unsigned char[_width * _height * 3]; // tga rgb buffer

		//unsigned int* sourcePtr = reinterpret_cast<unsigned int*>(sourcePixels);
		//unsigned char* destPtr = destPixels;
		//for( int i = 0; i < _width * _height; ++i ) {
		//	const unsigned int src = *(sourcePtr++);
		//	*(destPtr++) = unsigned char((src & 0xFF0000) >> 16); // red
		//	*(destPtr++) = unsigned char(src & 0xFF); // green
		//	*(destPtr++) = unsigned char((src & 0xFF00) >> 8); // blue
		//}

		//if( !TGA::writeToFile(file, _width, _height, destPixels, TGA::RGB, false) ) {
		//	delete[] destPixels;
		//	delete[] sourcePixels;
		//	stageTex->Release();
		//	return false;
		//}

		//delete[] destPixels;
		//delete[] sourcePixels;
		//stageTex->Release();




		//const int lines = _height;
		//auto srcPtr = reinterpret_cast<const unsigned char*>(map.pData);
		//const int rowPitch = _width * TextureFormat::bytesPerPixel(_format);
		////const int rowPitch = (_width * TextureFormat::bytesPerPixel(_format) + 7) / 8;
		//const int msize = std::min<int>(rowPitch, map.RowPitch);
		//unsigned char* pixels = new unsigned char[_width * _height * TextureFormat::bytesPerPixel(_format)];
		//unsigned char* dstPtr = pixels;
		//for( int h = 0; h < lines; ++h ) {
		//	memcpy_s(dstPtr, rowPitch, srcPtr, msize);
		//	srcPtr += map.RowPitch;
		//	dstPtr += rowPitch;
		//}
		//_device->getContext()->Unmap(stageTex, 0);
		//if( !TGA::writeToFile(file, _width, _height, pixels, TGA::RGB, true) ) {
		//	delete[] pixels;
		//	pixels = nullptr;
		//	stageTex->Release();
		//	return false;
		//}
		//delete[] pixels;
		//pixels = nullptr;
		//stageTex->Release();

		//D3D11_MAPPED_SUBRESOURCE map;
		//if( FAILED(_device->getContext()->Map(stageTex, 0, D3D11_MAP_READ, 0, &map)) ) {
		//	stageTex->Release();
		//	return false;
		//}

		//unsigned int* pixels = new unsigned int[_width * _height * TextureFormat::bytesPerPixel(_format)];
		//const int expectedRowSize = _width * TextureFormat::bytesPerPixel(_format);
		//if( expectedRowSize == map.RowPitch ) {
		//	bool b = true;
		//} else {
		//	auto srcPtr = reinterpret_cast<unsigned int*>(map.pData);
		//	const int rowPitch = _width * TextureFormat::bytesPerPixel(_format);
		//	const int msize = (rowPitch < map.RowPitch) ? rowPitch : map.RowPitch;
		//	for( int h = 0; h < _height; ++h ) {
		//		memcpy_s(pixels, rowPitch, srcPtr, msize);
		//		srcPtr += map.RowPitch;
		//		pixels += rowPitch;
		//	}
		//}

		//stageTex->Release();
		//delete[] pixels;

		//const UINT rowPitch = resource.RowPitch;
		//auto sourcePtr = reinterpret_cast<unsigned int*>(resource.pData);
		//unsigned char* pixels = new unsigned char[_width * _height * TextureFormat::bytesPerPixel(_format)];
		//const int bpp = TextureFormat::bytesPerPixel(_format);
		//int tgaCounter = 0;
		//for( int i = 0; i < (_width * _height * bpp); i += bpp ) {
		//	unsigned char value = sourcePtr[i];
		//	pixels[tgaCounter] = value;
		//	tgaCounter += bpp;
		//	//pixels[i] = sourcePtr[i];
		//}
		//_device->getContext()->Unmap(stageTex, 0);
		//stageTex->Release();

		//if( !TGA::writeToFile(file, _width, _height, pixels, TextureFormat::hasAlpha(_format) ? TGA::RGBA : TGA::RGB, true) ) {
		//	delete[] pixels;
		//	pixels = nullptr;
		//	return false;
		//}

		//delete[] pixels;
		//pixels = nullptr;

		//return true;
	}

	bool DXTexture2D::writeToDDS( const char* file ) {
		// todo: return ciri error codes instead of a boolean
		if( nullptr == file || nullptr == _texture2D ) {
			return false;
		}
		return SUCCEEDED(DirectX::SaveDDSTextureToFile(_device->getContext(), _texture2D, strutil::str2wstr(file).c_str()));
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