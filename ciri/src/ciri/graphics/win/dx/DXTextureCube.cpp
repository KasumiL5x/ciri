#include <ciri/graphics/win/dx/DXTextureCube.hpp>
#include <ciri/graphics/win/dx/DXGraphicsDevice.hpp>

using namespace ciri::graphics;
using namespace ciri::core;

DXTextureCube::DXTextureCube( const std::shared_ptr<DXGraphicsDevice>& device )
	: ITextureCube(), _device(device), _cubeTexture(nullptr), _shaderResourceView(nullptr) {
}

DXTextureCube::~DXTextureCube() {
	destroy();
}

ErrorCode DXTextureCube::set( int width, int height, void* posx, void* negx, void* posy, void* negy, void* posz, void* negz ) {
	// http://stackoverflow.com/questions/19364012/d3d11-creating-a-cube-map-from-6-images
	// http://www.braynzarsoft.net/index.php?p=D3D11CUBEMAP
	// http://www.hlsl.co.uk/blog/2014/11/19/creating-a-cubemap-in-dx11
		
	// must have positive dimensions
	if( width <= 0 || height <= 0 ) {
		return ErrorCode::CIRI_INVALID_ARGUMENT;
	}

	// must have valid images
	if( nullptr==posx || nullptr==negx || nullptr==posy || nullptr==negy || nullptr==posz || nullptr==negz ) {
		return ErrorCode::CIRI_INVALID_ARGUMENT;
	}

	// todo: update if already valid
	//if( _textureId != 0 ) {
		//return CIRI_NOT_IMPLEMENTED;
	//}

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 6;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MipLevels = texDesc.MipLevels;
	srvDesc.TextureCube.MostDetailedMip = 0;

	// note: top and bottom are currently <NOT> flipped
	void* ptrs[] = {posx, negx, posy, negy, posz, negz};
	D3D11_SUBRESOURCE_DATA data[6];
	for( int i = 0; i < 6; ++i ) {
		data[i].pSysMem = ptrs[i];
		data[i].SysMemPitch = width * 4;
		data[i].SysMemSlicePitch = 0;
	}

	if( FAILED(_device->getDevice()->CreateTexture2D(&texDesc, &data[0], &_cubeTexture)) ) {
		destroy();
		return ErrorCode::CIRI_UNKNOWN_ERROR;
	}

	if( FAILED(_device->getDevice()->CreateShaderResourceView(_cubeTexture, &srvDesc, &_shaderResourceView)) ) {
		destroy();
		return ErrorCode::CIRI_UNKNOWN_ERROR;
	}

	return ErrorCode::CIRI_OK;
}

void DXTextureCube::destroy() {
	if( _shaderResourceView != nullptr ) {
		_shaderResourceView->Release();
		_shaderResourceView = nullptr;
	}

	if( _cubeTexture != nullptr ) {
		_cubeTexture->Release();
		_cubeTexture = nullptr;
	}
}

ID3D11Texture2D* DXTextureCube::getTexture() const {
	return _cubeTexture;
}

ID3D11ShaderResourceView* DXTextureCube::getShaderResourceView() const {
	return _shaderResourceView;
}