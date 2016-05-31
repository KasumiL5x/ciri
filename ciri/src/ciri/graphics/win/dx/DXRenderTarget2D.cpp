#include <ciri/graphics/win/dx/DXRenderTarget2D.hpp>
#include <ciri/graphics/win/dx/DXGraphicsDevice.hpp>
#include <ciri/graphics/win/dx/DXTexture2D.hpp>

using namespace ciri;

DXRenderTarget2D::DXRenderTarget2D( const std::shared_ptr<DXGraphicsDevice>& device )
	: IRenderTarget2D(), _device(device), _texture(nullptr), _renderTargetView(nullptr) {
}

DXRenderTarget2D::~DXRenderTarget2D() {
	destroy();
}

bool DXRenderTarget2D::create( const std::shared_ptr<DXTexture2D>& texture ) {
	if( _renderTargetView != nullptr ) {
		return false;
	}

	if( nullptr == texture ) {
		return false;
	}

	_texture = texture;
		
	// get the texture's description
	D3D11_TEXTURE2D_DESC textureDesc;
	texture->getTexture()->GetDesc(&textureDesc);

	// create render target description
	D3D11_RENDER_TARGET_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Format = textureDesc.Format;
	desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice = 0;

	if( FAILED(_device->getDevice()->CreateRenderTargetView(texture->getTexture(), &desc, &_renderTargetView)) ) {
		return false;
	}
	return true;
}

void DXRenderTarget2D::destroy() {
	if( _renderTargetView != nullptr ) {
		_renderTargetView->Release();
		_renderTargetView = nullptr;
	}
}

std::shared_ptr<ITexture2D> DXRenderTarget2D::getTexture() const {
	return _texture;
}

std::shared_ptr<ITexture2D> DXRenderTarget2D::getDepth() const {
	throw;
}

ID3D11RenderTargetView* DXRenderTarget2D::getRenderTargetView() const {
	return _renderTargetView;
}