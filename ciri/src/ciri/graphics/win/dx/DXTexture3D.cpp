#include <ciri/graphics/win/dx/DXTexture3D.hpp>
#include <ciri/graphics/win/dx/DXGraphicsDevice.hpp>
#include <ciri/graphics/win/dx/CiriToDx.hpp>

using namespace ciri;

DXTexture3D::DXTexture3D( int flags, const std::shared_ptr<DXGraphicsDevice>& device )
	: ITexture3D(flags), _device(device), _flags(flags), _format(TextureFormat::RGBA32_UINT), _texture3D(nullptr), _shaderResourceView(nullptr), _width(0), _height(0), _depth(0) {
}

DXTexture3D::~DXTexture3D() {
}

void DXTexture3D::destroy() {
	if( _texture3D != nullptr ) {
		_texture3D->Release();
		_texture3D = nullptr;
	}

	if( _shaderResourceView != nullptr ) {
		_shaderResourceView->Release();
		_shaderResourceView = nullptr;
	}
}

ErrorCode DXTexture3D::setData( int width, int height, int depth, void* data, TextureFormat::Format format ) {
	// update if already valid
	if( _shaderResourceView != nullptr ) {
		return ErrorCode::CIRI_NOT_IMPLEMENTED;
	}

	// store dimensions
	_width = width;
	_height = height;
	_depth = depth;

	// store format
	_format = format;

	// create the texture description
	D3D11_TEXTURE3D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.Depth = depth;
	texDesc.MipLevels = getMipLevels();
	texDesc.Format = ciriToDxTextureFormat(format);
	texDesc.Usage = getUsage();
	texDesc.BindFlags = getBindFlags();
	texDesc.CPUAccessFlags = getCpuFlags();
	texDesc.MiscFlags = getMiscFlags();

	// multisampled textures require initialization with nullptr first, so just do it this way for everything for simplicity
	if( FAILED(_device->getDevice()->CreateTexture3D(&texDesc, nullptr, &_texture3D)) ) {
		destroy();
		return ErrorCode::CIRI_UNKNOWN_ERROR; // todo: texture create failure
	}

	// todo: set pixels
	if( data != nullptr ) {
		const UINT pitch = width * TextureFormat::bytesPerPixel(format);
		const UINT depth = width * height * TextureFormat::bytesPerPixel(format);
		_device->getContext()->UpdateSubresource(_texture3D, 0, nullptr, data, pitch, depth);
	}

	// create actual shader resource view
	if( FAILED(_device->getDevice()->CreateShaderResourceView(_texture3D, nullptr, &_shaderResourceView)) ) {
		destroy();
		return ErrorCode::CIRI_UNKNOWN_ERROR; // todo: texture create failure
	}

	// generate mipmaps
	if( _flags & TextureFlags::Mipmaps ) {
		_device->getContext()->GenerateMips(_shaderResourceView);
	}

	return ErrorCode::CIRI_OK;
}

int DXTexture3D::getWidth() const {
	return _width;
}

int DXTexture3D::getHeight() const {
	return _height;
}

int DXTexture3D::getDepth() const {
	return _depth;
}

TextureFormat::Format DXTexture3D::getFormat() const {
	return _format;
}

ErrorCode DXTexture3D::writeToTGA( const char* file ) {
	return ErrorCode::CIRI_NOT_IMPLEMENTED;
}

ErrorCode DXTexture3D::writeToDDS( const char* file ) {
	return ErrorCode::CIRI_NOT_IMPLEMENTED;
}

ID3D11Texture3D* DXTexture3D::getTexture() const {
	return _texture3D;
}

ID3D11ShaderResourceView* DXTexture3D::getShaderResourceView() const {
	return _shaderResourceView;
}

UINT DXTexture3D::getMipLevels() const {
	return (_flags & TextureFlags::Mipmaps) ? 0 : 1;
}

D3D11_USAGE DXTexture3D::getUsage() const {
	if( _flags & TextureFlags::RenderTarget ) {
		return D3D11_USAGE_DEFAULT;
	}

	if( _flags & TextureFlags::Mipmaps ) {
		return D3D11_USAGE_DEFAULT;
	}

	// todo: if editable, return dynamic

	return D3D11_USAGE_DEFAULT;
}

UINT DXTexture3D::getBindFlags() const {
	UINT bindFlags = D3D11_BIND_SHADER_RESOURCE;

	if( _flags & TextureFlags::RenderTarget ) {
		bindFlags |= D3D11_BIND_RENDER_TARGET;
	}

	if( _flags & TextureFlags::Mipmaps ) {
		bindFlags |= D3D11_BIND_RENDER_TARGET;
	}

	return bindFlags;
}

UINT DXTexture3D::getCpuFlags() const {
	UINT cpuFlags = 0;
	// D3D11_CPU_ACCESS_WRITE ?
	// D3D11_CPU_ACCESS_READ ?
	return cpuFlags;
}

UINT DXTexture3D::getMiscFlags() const {
	UINT miscFlags = 0;
	if( _flags & TextureFlags::Mipmaps ) {
		miscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}
	return miscFlags;
}