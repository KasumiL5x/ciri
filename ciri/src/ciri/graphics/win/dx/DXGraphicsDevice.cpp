#include <ciri/graphics/win/dx/DXGraphicsDevice.hpp>
#include <ciri/core/window/IWindow.hpp>
#include <ciri/graphics/win/dx/DXConstantBuffer.hpp>
#include <ciri/graphics/win/dx/DXTexture2D.hpp>
#include <ciri/graphics/win/dx/DXTexture3D.hpp>
#include <ciri/graphics/win/dx/DXTextureCube.hpp>
#include <ciri/graphics/win/dx/DXSamplerState.hpp>
#include <ciri/graphics/win/dx/DXRenderTarget2D.hpp>
#include <ciri/graphics/win/dx/DXBlendState.hpp>
#include <ciri/graphics/ClearFlags.hpp>
#include <ciri/core/StrUtil.hpp>
#include <ciri/graphics/win/dx/CiriToDx.hpp>

using namespace ciri;

DXGraphicsDevice::DXGraphicsDevice()
	: IGraphicsDevice(), _isValid(false), _window(nullptr), _swapchain(nullptr), _device(nullptr), _context(nullptr), _backbuffer(nullptr),
		_defaultWidth(0), _defaultHeight(0),
		_depthStencil(nullptr), _depthStencilView(nullptr), _shaderExt(".hlsl") {
}

DXGraphicsDevice::~DXGraphicsDevice() {
	destroy();
}

bool DXGraphicsDevice::create( const std::shared_ptr<IWindow>& window ) {
	if( _isValid ) {
		return false;
	}

	_window = window;

	_defaultWidth = window->getWidth();
	_defaultHeight = window->getHeight();

	if( !initDevice(window->getWidth(), window->getHeight(), static_cast<HWND>(window->getNativeHandle())) ) {
		destroy();
		return false;
	}

	for( int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i ) {
		_activeRenderTargets[i] = nullptr;
	}

	_clearColor[0] = 0.39f;
	_clearColor[1] = 0.58f;
	_clearColor[2] = 0.93f;
	_clearColor[3] = 1.0f;

	_isValid = true;

	// set default states
	restoreDefaultStates();

	return true;
}

void DXGraphicsDevice::destroy() {
	if( !_isValid ) {
		return;
	}

	// clean default state pointers
	_defaultBlendAdditive = nullptr;
	_defaultBlendAlpha = nullptr;
	_defaultBlendNonPremul = nullptr;
	_defaultBlendOpaque = nullptr;
	_defaultRasterNone = nullptr;
	_defaultRasterClockwise = nullptr;
	_defaultRasterCounterClockwise = nullptr;
	_defaultDepthStencilDefault = nullptr;
	_defaultDepthStencilDepthRead = nullptr;
	_defaultDepthStencilNone = nullptr;

	if( _depthStencil != nullptr ) { _depthStencil->Release(); _depthStencil = nullptr; }
	if( _depthStencilView != nullptr ) { _depthStencilView->Release(); _depthStencilView = nullptr; }
	if( _backbuffer ) { _backbuffer->Release(); _backbuffer = nullptr; }
	if( _context )    { _context->ClearState(); _context = nullptr; }
	if( _swapchain )  { _swapchain->Release(); _swapchain = nullptr; }
	if( _device )     { _device->Release(); _device = nullptr; }

	_isValid = false;
}

void DXGraphicsDevice::present() {
	if( !_isValid ) {
		return;
	}

	_swapchain->Present(0, 0);
}

void DXGraphicsDevice::setViewport( const Viewport& vp ) {
	if( nullptr == _context ) {
		return;
	}

	D3D11_VIEWPORT dxVp;
	dxVp.TopLeftX = static_cast<float>(vp.x());
	dxVp.TopLeftY = static_cast<float>(vp.y());
	dxVp.Width = static_cast<float>(vp.width());
	dxVp.Height = static_cast<float>(vp.height());
	dxVp.MinDepth = vp.minDepth();
	dxVp.MaxDepth = vp.maxDepth();
	_context->RSSetViewports(1, &dxVp);

	_activeViewport = vp;
}

const Viewport& DXGraphicsDevice::getViewport() const {
	return _activeViewport;
}

std::shared_ptr<IShader> DXGraphicsDevice::createShader() {
	if( !_isValid ) {
		return nullptr;
	}

	std::shared_ptr<DXShader> shader = std::make_shared<DXShader>(shared_from_this());
	return shader;
}

std::shared_ptr<IVertexBuffer> DXGraphicsDevice::createVertexBuffer() {
	if( !_isValid ) {
		return nullptr;
	}

	std::shared_ptr<DXVertexBuffer> buffer = std::make_shared<DXVertexBuffer>(shared_from_this());
	return buffer;
}

std::shared_ptr<IIndexBuffer> DXGraphicsDevice::createIndexBuffer() {
	if( !_isValid ) {
		return nullptr;
	}

	std::shared_ptr<DXIndexBuffer> buffer = std::make_shared<DXIndexBuffer>(shared_from_this());
	return buffer;
}

std::shared_ptr<IConstantBuffer> DXGraphicsDevice::createConstantBuffer() {
	if( !_isValid ) {
		return nullptr;
	}

	std::shared_ptr<DXConstantBuffer> buffer = std::make_shared<DXConstantBuffer>(shared_from_this());
	return buffer;
}

std::shared_ptr<ITexture2D> DXGraphicsDevice::createTexture2D( int width, int height, TextureFormat::Format format, int flags, void* pixels ) {
	if( !_isValid ) {
		return nullptr;
	}

	if( width <= 0 || height <= 0 ) {
		return nullptr;
	}

	std::shared_ptr<DXTexture2D> dxTexture = std::make_shared<DXTexture2D>(flags, shared_from_this());
	if( failed(dxTexture->setData(0, 0, width, height, pixels, format)) ) {
		dxTexture.reset();
		dxTexture = nullptr;
		return nullptr;
	}

	return dxTexture;
}

std::shared_ptr<ITexture3D> DXGraphicsDevice::createTexture3D( int width, int height, int depth, TextureFormat::Format format, int flags, void* pixels ) {
	if( !_isValid ) {
		return nullptr;
	}

	if( width <= 0 || height <= 0 || depth <= 0 ) {
		return nullptr;
	}

	std::shared_ptr<DXTexture3D> dxTexture = std::make_shared<DXTexture3D>(flags, shared_from_this());
	if( failed(dxTexture->setData(width, height, depth, pixels, format)) ) {
		dxTexture.reset();
		dxTexture = nullptr;
		return nullptr;
	}

	return dxTexture;
}

std::shared_ptr<ITextureCube> DXGraphicsDevice::createTextureCube( int width, int height, void* posx, void* negx, void* posy, void* negy, void* posz, void* negz ) {
	if( width <= 0 || height <= 0 ) {
		return nullptr;
	}

	if( nullptr==posx || nullptr==negx || nullptr==posy || nullptr==negy || nullptr==posz || nullptr==negz ) {
		return nullptr;
	}

	std::shared_ptr<DXTextureCube> dxCube = std::make_shared<DXTextureCube>(shared_from_this());
	if( failed(dxCube->set(width, height, posx, negx, posy, negy, posz, negz)) ) {
		dxCube.reset();
		dxCube = nullptr;
		return nullptr;
	}

	return dxCube;
}

std::shared_ptr<ISamplerState> DXGraphicsDevice::createSamplerState( const SamplerDesc& desc ) {
	if( !_isValid ) {
		return nullptr;
	}

	std::shared_ptr<DXSamplerState> dxSampler = std::make_shared<DXSamplerState>(shared_from_this());
	if( !dxSampler->create(desc) ) {
		dxSampler.reset();
		dxSampler = nullptr;
		return nullptr;
	}
	return dxSampler;
}

std::shared_ptr<IRenderTarget2D> DXGraphicsDevice::createRenderTarget2D( int width, int height, TextureFormat::Format format, DepthFormat depthFormat ) {
	if( !_isValid ) {
		return nullptr;
	}

	std::shared_ptr<DXTexture2D> texture = std::static_pointer_cast<DXTexture2D>(this->createTexture2D(width, height, format, TextureFlags::RenderTarget, nullptr));
	if( nullptr == texture ) {
		return nullptr;
	}
	std::shared_ptr<DXRenderTarget2D> dxTarget = std::make_shared<DXRenderTarget2D>(shared_from_this());
	if( !dxTarget->create(texture) ) {
		texture->destroy();
		texture.reset();
		texture = nullptr;
		dxTarget.reset();
		dxTarget = nullptr;
	}
	return dxTarget;
}

std::shared_ptr<IRasterizerState> DXGraphicsDevice::createRasterizerState( const RasterizerDesc& desc ) {
	if( !_isValid ) {
		return nullptr;
	}

	std::shared_ptr<DXRasterizerState> dxRaster = std::make_shared<DXRasterizerState>(shared_from_this());
	if( !dxRaster->create(desc) ) {
		dxRaster.reset();
		dxRaster = nullptr;
		return nullptr;
	}
	return dxRaster;
}

std::shared_ptr<IDepthStencilState> DXGraphicsDevice::createDepthStencilState( const DepthStencilDesc& desc ) {
	if( !_isValid ) {
		return nullptr;
	}

	std::shared_ptr<DXDepthStencilState> dxState = std::make_shared<DXDepthStencilState>(shared_from_this());
	if( !dxState->create(desc) ) {
		dxState.reset();
		dxState = nullptr;
		return nullptr;
	}
	return dxState;
}

std::shared_ptr<IBlendState> DXGraphicsDevice::createBlendState( const BlendDesc& desc ) {
	if( !_isValid ) {
		return nullptr;
	}

	std::shared_ptr<DXBlendState> dxState = std::make_shared<DXBlendState>(shared_from_this());
	if( !dxState->create(desc) ) {
		dxState.reset();
		dxState = nullptr;
		return nullptr;
	}
	return dxState;
}

void DXGraphicsDevice::applyShader( const std::shared_ptr<IShader>& shader ) {
	if( !_isValid ) {
		return;
	}

	if( nullptr == shader ) {
		_context->VSSetShader(nullptr, nullptr, 0);
		_context->GSSetShader(nullptr, nullptr, 0);
		_context->PSSetShader(nullptr, nullptr, 0);
		_activeShader.reset();
		return;
	}

	if( !shader->isValid() ) {
		_activeShader.reset();
		return;
	}

	const std::shared_ptr<DXShader> dxShader = std::static_pointer_cast<DXShader>(shader);

	ID3D11VertexShader* vs = dxShader->getVertexShader();
	_context->VSSetShader(vs, nullptr, 0);
	const std::vector<std::shared_ptr<DXConstantBuffer>>& vertexConstants = dxShader->getVertexConstants();
	for( unsigned int i = 0; i < vertexConstants.size(); ++i ) {
		ID3D11Buffer* buffer = vertexConstants[i]->getBuffer();
		const int index = vertexConstants[i]->getIndex();
		_context->VSSetConstantBuffers(index, 1, &buffer);
	}

	ID3D11GeometryShader* gs = dxShader->getGeometryShader();
	if( gs != nullptr ) {
		_context->GSSetShader(gs, nullptr, 0);
		const std::vector<std::shared_ptr<DXConstantBuffer>>& geometryConstants = dxShader->getGeometryConstants();
		for( unsigned int i = 0; i < geometryConstants.size(); ++i ) {
			ID3D11Buffer* buffer = geometryConstants[i]->getBuffer();
			const int index = geometryConstants[i]->getIndex();
			_context->GSSetConstantBuffers(index, 1, &buffer);
		}
	} else {
		_context->GSSetShader(nullptr, nullptr, 0); // disable non-existent gs in case previous shader had it applied
	}

	ID3D11PixelShader* ps = dxShader->getPixelShader();
	_context->PSSetShader(ps, nullptr, 0);
	const std::vector<std::shared_ptr<DXConstantBuffer>>& pixelConstants = dxShader->getPixelConstants();
	for( unsigned int i = 0; i < pixelConstants.size(); ++i ) {
		ID3D11Buffer* buffer = pixelConstants[i]->getBuffer();
		const int index = pixelConstants[i]->getIndex();
		_context->PSSetConstantBuffers(index, 1, &buffer);
	}

	_activeShader = dxShader;
}

void DXGraphicsDevice::setVertexBuffer( const std::shared_ptr<IVertexBuffer>& buffer ) {
	if( !_isValid ) {
		return;
	}

	// make dx match it although IASetInputLayout could really be called at any other time...(see below)
	if( _activeShader.expired() ) {
		return;
	}

	if( nullptr == buffer ) {
		_context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
		return;
	}

	const std::shared_ptr<DXVertexBuffer> dxBuffer = std::static_pointer_cast<DXVertexBuffer>(buffer);
	UINT stride = buffer->getStride();
	UINT offset = 0;
	ID3D11Buffer* vb = dxBuffer->getVertexBuffer();
	if( nullptr == vb ) {
		return;
	}
	_context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

	// set input layout here as current GL solution requires vertex array to be bound when setting vertex attributes (hence cannot set in applyShader)
	ID3D11InputLayout* il = _activeShader.lock()->getInputLayout();
	_context->IASetInputLayout(il);

	_activeVertexBuffer = dxBuffer;
}

void DXGraphicsDevice::setIndexBuffer( const std::shared_ptr<IIndexBuffer>& buffer ) {
	if( !_isValid ) {
		return;
	}

	if( nullptr == buffer ) {
		_context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		return;
	}

	const std::shared_ptr<DXIndexBuffer> dxBuffer = std::static_pointer_cast<DXIndexBuffer>(buffer);

	ID3D11Buffer* ib = dxBuffer->getIndexBuffer();
	if( nullptr == ib ) {
		//_activeIndexBuffer.reset();
		return; // todo: error
	}
	_context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

	_activeIndexBuffer = dxBuffer;
}

void DXGraphicsDevice::setTexture2D( int index, const std::shared_ptr<ITexture2D>& texture, ShaderStage::Stage shaderStage ) {
	if( !_isValid ) {
		return;
	}

	if( index < 0 ) {
		return;
	}

	const std::shared_ptr<DXTexture2D> dxTexture = std::static_pointer_cast<DXTexture2D>(texture);

	// has to be an "array" to clear targets (if the input texture is nullptr) or dx shits its pants
	ID3D11ShaderResourceView* srv[1] = { (texture != nullptr) ? dxTexture->getShaderResourceView() : nullptr };

	const bool all = (shaderStage & ShaderStage::All) != 0;
	if( all || (shaderStage & ShaderStage::Vertex) ) {
		_context->VSSetShaderResources(index, 1, srv);
	}
	if( all || (shaderStage & ShaderStage::Geometry) ) {
		_context->GSSetShaderResources(index, 1, srv);
	}
	if( all || (shaderStage & ShaderStage::Pixel) ) {
		_context->PSSetShaderResources(index, 1, srv);
	}
}

void DXGraphicsDevice::setTexture3D( int index, const std::shared_ptr<ITexture3D>& texture, ShaderStage::Stage shaderStage ) {
	if( !_isValid ) {
		return;
	}

	if( index < 0 ) {
		return;
	}

	const std::shared_ptr<DXTexture3D> dxTexture = std::static_pointer_cast<DXTexture3D>(texture);

	// has to be an "array" to clear targets (if the input texture is nullptr) or dx shits its pants
	ID3D11ShaderResourceView* srv[1] = { (texture != nullptr) ? dxTexture->getShaderResourceView() : nullptr };

	const bool all = (shaderStage & ShaderStage::All) != 0;
	if( all || (shaderStage & ShaderStage::Vertex) ) {
		_context->VSSetShaderResources(index, 1, srv);
	}
	if( all || (shaderStage & ShaderStage::Geometry) ) {
		_context->GSSetShaderResources(index, 1, srv);
	}
	if( all || (shaderStage & ShaderStage::Pixel) ) {
		_context->PSSetShaderResources(index, 1, srv);
	}
}

void DXGraphicsDevice::setTextureCube( int index, const std::shared_ptr<ITextureCube>& texture, ShaderStage::Stage shaderStage ) {
	if( !_isValid ) {
		return;
	}

	const std::shared_ptr<DXTextureCube> dxTexture = std::static_pointer_cast<DXTextureCube>(texture);

	ID3D11ShaderResourceView* srv[1] = { (texture != nullptr) ? dxTexture->getShaderResourceView() : nullptr };

	const bool all = (shaderStage & ShaderStage::All) != 0;
	if( all || (shaderStage & ShaderStage::Vertex) ) {
		_context->VSSetShaderResources(index, 1, srv);
	}
	if( all || (shaderStage & ShaderStage::Geometry) ) {
		_context->GSSetShaderResources(index, 1, srv);
	}
	if( all || (shaderStage & ShaderStage::Pixel) ) {
		_context->PSSetShaderResources(index, 1, srv);
	}
}

void DXGraphicsDevice::setSamplerState( int index, const std::shared_ptr<ISamplerState>& state, ShaderStage::Stage shaderStage ) {
	if( !_isValid ) {
		return;
	}

	const std::shared_ptr<DXSamplerState> dxSampler = std::static_pointer_cast<DXSamplerState>(state);

	// has to be an "array" to clear targets (if the input texture is nullptr) or dx shits its pants
	ID3D11SamplerState* sampler[1] = { (state != nullptr) ? dxSampler->getSamplerState() : nullptr };

	const bool all = (shaderStage & ShaderStage::All) != 0;
	if( all || (shaderStage & ShaderStage::Vertex) ) {
		_context->VSSetSamplers(index, 1, sampler);
	}
	if( all || (shaderStage & ShaderStage::Geometry) ) {
		_context->GSSetSamplers(index, 1, sampler);
	}
	if( all || (shaderStage & ShaderStage::Pixel) ) {
		_context->PSSetSamplers(index, 1, sampler);
	}
}

void DXGraphicsDevice::setBlendState( const std::shared_ptr<IBlendState>& state ) {
	if( !_isValid ) {
		return;
	}

	if( nullptr == state ) {
		restoreDefaultBlendState();
		//_context->OMSetBlendState(nullptr, nullptr, 0xffffffff);
		return;
	}

	const std::shared_ptr<DXBlendState> dxState = std::static_pointer_cast<DXBlendState>(state);
	ID3D11BlendState* bs = dxState->getBlendState();
	_context->OMSetBlendState(bs, dxState->getDesc().blendFactor, 0xffffffff); // todo: sample mask support?
}

void DXGraphicsDevice::drawArrays( PrimitiveTopology topology, int vertexCount, int startIndex ) {
	if( !_isValid ) {
		return;
	}

	// cannot draw with no active shader
	if( _activeShader.expired() ) {
		return;
	}

	// cannot draw with no active vertex buffer
	if( _activeVertexBuffer.expired() ) {
		return;
	}

	// vertex count must be greater than 0!
	if( vertexCount <= 0 ) {
		return;
	}

	// start index must be greater than zero and less than vertex count
	//if( startIndex < 0 || startIndex >= vertexCount ) {
		//return;
	//}

	_context->IASetPrimitiveTopology(ciriToDxTopology(topology));
	_context->Draw(vertexCount, startIndex);
}

void DXGraphicsDevice::drawIndexed( PrimitiveTopology topology, int indexCount ) {
	if( !_isValid ) {
		return;
	}

	// cannot draw with no active shader
	if( _activeShader.expired() ) {
		return;
	}

	// cannot draw without a valid vertex and index buffer
	if( _activeVertexBuffer.expired() || _activeIndexBuffer .expired() ) {
		return;
	}

	// index count must be greater than 0
	if( indexCount <= 0 ) {
		return; // todo: error
	}

	_context->IASetPrimitiveTopology(ciriToDxTopology(topology));
	_context->DrawIndexed(indexCount, 0, 0);
}
	
void DXGraphicsDevice::setRenderTargets( IRenderTarget2D** renderTargets, int numRenderTargets ) {
	if( !_isValid ) {
		return;
	}

	// build array of pointers to render targets
	for( int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i ) {
		if( i >= numRenderTargets ) {
			_activeRenderTargets[i] = nullptr;
			continue;
		}
		_activeRenderTargets[i] = reinterpret_cast<DXRenderTarget2D*>(renderTargets[i])->getRenderTargetView();
	}

	// todo: take either a separate depth render target, OR take the first render target's depth render target,
	//       as by convention, they should all have the same format depth target
	_context->OMSetRenderTargets(numRenderTargets, _activeRenderTargets, nullptr);

	setViewport(Viewport(0, 0, renderTargets[0]->getTexture()->getWidth(), renderTargets[0]->getTexture()->getHeight()));
}

void DXGraphicsDevice::restoreDefaultRenderTargets() {
	if( !_isValid ) {
		return;
	}

	for( int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i ) {
		_activeRenderTargets[i] = nullptr;
	}
	_context->OMSetRenderTargets(1, &_backbuffer, _depthStencilView);

	setViewport(Viewport(0, 0, _defaultWidth, _defaultHeight));
}

ErrorCode DXGraphicsDevice::resize() {
	if( !_isValid ) {
		return ErrorCode::CIRI_UNKNOWN_ERROR;
	}

	// todo: check for erroneous sizes

	// get new window size
	const int width = _window->getWidth();
	const int height = _window->getHeight();

	// don't resize if the same size
	if( width == _defaultWidth && height == _defaultHeight ) {
		return ErrorCode::CIRI_OK; // not incorrect; just ignore it
	}

	// update default width and height
	_defaultWidth = width;
	_defaultHeight = height;

	// remove all render targets
	_context->OMSetRenderTargets(0, 0, 0);
	// release backbuffer pointers
	_backbuffer->Release();
	// resize buffers
	if( FAILED(_swapchain->ResizeBuffers(0, _defaultWidth, _defaultHeight, DXGI_FORMAT_UNKNOWN, 0)) ) {
		return ErrorCode::CIRI_UNKNOWN_ERROR;
	}
	// recreate RTV for backbuffer
	if( !createBackbufferRtv() ) {
		return ErrorCode::CIRI_UNKNOWN_ERROR;
	}

	// release depth stencil and its texture
	_depthStencilView->Release();
	_depthStencil->Release();
	// recreate depth stencil and its texture
	if( !createDepthStencilView() ) {
		return ErrorCode::CIRI_UNKNOWN_ERROR;
	}

	// set render target back to backbuffer
	_context->OMSetRenderTargets(1, &_backbuffer, _depthStencilView);
	// create and set the new viewport
	setViewport(Viewport(0, 0, width, height));

	return ErrorCode::CIRI_OK;
}

ErrorCode DXGraphicsDevice::resizeTexture2D( const std::shared_ptr<ITexture2D>& texture, int width, int height )  {
	if( !_isValid ) {
		return ErrorCode::CIRI_UNKNOWN_ERROR;
	}

	if( width <= 0 || height <= 0 || nullptr == texture ) {
		return ErrorCode::CIRI_INVALID_ARGUMENT;
	}

	const std::shared_ptr<DXTexture2D> dxTexture = std::static_pointer_cast<DXTexture2D>(texture);
		
	// check for texture that isn't created
	if( nullptr == dxTexture->getTexture() ) {
		return ErrorCode::CIRI_UNKNOWN_ERROR;
	}

	dxTexture->destroy();
	return dxTexture->setData(0, 0, width, height, nullptr, dxTexture->getFormat());
}

ErrorCode DXGraphicsDevice::resizeRenderTarget2D( const std::shared_ptr<IRenderTarget2D>& target, int width, int height ) {
	if( !_isValid ) {
		return ErrorCode::CIRI_UNKNOWN_ERROR;
	}

	if( width <= 0 || height <= 0 || nullptr == target ) {
		return ErrorCode::CIRI_INVALID_ARGUMENT;
	}

	const std::shared_ptr<DXRenderTarget2D> dxTarget = std::static_pointer_cast<DXRenderTarget2D>(target);

	// check for texure that isn't created
	if( nullptr == dxTarget->getTexture() ) {
		return ErrorCode::CIRI_UNKNOWN_ERROR;
	}

	// store texture's format for re-creation
	const TextureFormat::Format textureFormat = dxTarget->getTexture()->getFormat();

	dxTarget->destroy();
	const std::shared_ptr<DXTexture2D> dxTexture = std::static_pointer_cast<DXTexture2D>(createTexture2D(width, height, textureFormat, TextureFlags::RenderTarget, nullptr));
	if( nullptr == dxTexture ) {
		return ErrorCode::CIRI_UNKNOWN_ERROR;
	}
	return dxTarget->create(dxTexture) ? ErrorCode::CIRI_OK : ErrorCode::CIRI_UNKNOWN_ERROR;
}

void DXGraphicsDevice::setClearColor( float r, float g, float b, float a ) {
	if( !_isValid ) { // gl requires an active context, so behave the same in dx
		return;
	}

	_clearColor[0] = r;
	_clearColor[1] = g;
	_clearColor[2] = b;
	_clearColor[3] = a;
}

void DXGraphicsDevice::clear( int flags ) {
	if( !_isValid ) {
		return;
	}

	// todo: add set depth and stencil clear value functions
	if( nullptr == _activeRenderTargets[0] ) {
		if( flags & ClearFlags::Color ) {
			_context->ClearRenderTargetView(_backbuffer, _clearColor);
		}
		if( (flags & ClearFlags::Depth) && (flags & ClearFlags::Stencil) ) {
			_context->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		} else if( flags & ClearFlags::Depth ) {
			_context->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		} if( flags & ClearFlags::Stencil ) {
			_context->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
		return;
	}

	for( int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i ) {
		if( _activeRenderTargets[i] != nullptr ) {
			if( flags & ClearFlags::Color ) {
				_context->ClearRenderTargetView(_activeRenderTargets[i], _clearColor);
			}
		}
	}
	// todo: once i get an _activeDepthRenderTarget variable added, do something akin to the following:
	//if( _activeDepthRenderTarget != nullptr ) {
	//	if( (flags & ClearFlags::Depth) && (flags & ClearFlags::Stencil) ) {
	//		_context->ClearDepthStencilView(_activeDepthRenderTarget, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//	} else if( flags & ClearFlags::Depth ) {
	//		_context->ClearDepthStencilView(_activeDepthRenderTarget, D3D11_CLEAR_DEPTH, 1.0f, 0);
	//	} if( flags & ClearFlags::Stencil ) {
	//		_context->ClearDepthStencilView(_activeDepthRenderTarget, D3D11_CLEAR_STENCIL, 1.0f, 0);
	//	}
	//}
}

void DXGraphicsDevice::setRasterizerState( const std::shared_ptr<IRasterizerState>& state ) {
	if( !_isValid ) {
		return;
	}

	if( nullptr == state ) {
		restoreDefaultRasterizerState();
		return;
	}

	const std::shared_ptr<DXRasterizerState> dxRaster = std::static_pointer_cast<DXRasterizerState>(state);
	if( !_activeRasterizerState.expired() && (dxRaster == _activeRasterizerState.lock()) ) {
		return;
	}
	_activeRasterizerState = dxRaster;
	ID3D11RasterizerState* dxState = dxRaster->getRasterizerState();
	_context->RSSetState(dxState);
}

void DXGraphicsDevice::setDepthStencilState( const std::shared_ptr<IDepthStencilState>& state ) {
	if( !_isValid ) {
		return;
	}

	if( nullptr == state ) {
		restoreDefaultDepthStencilState();
		return;
	}

	const std::shared_ptr<DXDepthStencilState> dxState = std::static_pointer_cast<DXDepthStencilState>(state);
	if( !_activeDepthStencilState.expired() && (dxState == _activeDepthStencilState.lock()) ) {
		return;
	}
	_activeDepthStencilState = dxState;
	ID3D11DepthStencilState* dxDepthStencilState = dxState->getState();
	_context->OMSetDepthStencilState(dxDepthStencilState, dxState->getStencilRef());
}

void DXGraphicsDevice::setShaderExt( const char* ext ) {
	_shaderExt = ext;
}

const char* DXGraphicsDevice::getShaderExt() const {
	return _shaderExt.c_str();
}

std::shared_ptr<IWindow> DXGraphicsDevice::getWindow() const {
	return _window;
}

const char* DXGraphicsDevice::getGpuName() const {
	return _gpuName.c_str();
}

const char* DXGraphicsDevice::getApiInfo() const {
	return _apiInfo.c_str();
}

GraphicsApiType DXGraphicsDevice::getApiType() const {
	return GraphicsApiType::DirectX11;
}

ErrorCode DXGraphicsDevice::restoreDefaultStates() {
	// restore blend state
	ErrorCode status = restoreDefaultBlendState();
	if( failed(status) ) {
		return status;
	}
	// restore rasterizer state
	status = restoreDefaultRasterizerState();
	if( failed(status) ) {
		return status;
	}
	// restore depth stencil state
	status = restoreDefaultDepthStencilState();
	if( failed(status) ) {
		return status;
	}
	return ErrorCode::CIRI_OK;
}

ErrorCode DXGraphicsDevice::restoreDefaultBlendState() {
	setBlendState(getDefaultBlendOpaque());
	return ErrorCode::CIRI_OK;
}

ErrorCode DXGraphicsDevice::restoreDefaultRasterizerState() {
	setRasterizerState(getDefaultRasterCounterClockwise());
	return ErrorCode::CIRI_OK;
}

ErrorCode DXGraphicsDevice::restoreDefaultDepthStencilState() {
	setDepthStencilState(getDefaultDepthStencilDefault());
	return ErrorCode::CIRI_OK;
}

std::shared_ptr<IBlendState> DXGraphicsDevice::getDefaultBlendAdditive() {
	if( nullptr == _defaultBlendAdditive ) {
		BlendDesc desc;
		desc.srcColorBlend = BlendMode::SourceAlpha;
		desc.srcAlphaBlend = BlendMode::SourceAlpha;
		desc.dstColorBlend = BlendMode::One;
		desc.dstAlphaBlend = BlendMode::One;
		_defaultBlendAdditive = createBlendState(desc);
	}
	return _defaultBlendAdditive;
}

std::shared_ptr<IBlendState> DXGraphicsDevice::getDefaultBlendAlpha() {
	if( nullptr == _defaultBlendAlpha ) {
		BlendDesc desc;
		desc.srcColorBlend = BlendMode::One;
		desc.srcAlphaBlend = BlendMode::One;
		desc.dstColorBlend = BlendMode::InverseSourceAlpha;
		desc.dstAlphaBlend = BlendMode::InverseSourceAlpha;
		_defaultBlendAlpha = createBlendState(desc);
	}
	return _defaultBlendAlpha;
}

std::shared_ptr<IBlendState> DXGraphicsDevice::getDefaultBlendNonPremul() {
	if( nullptr == _defaultBlendNonPremul ) {
		BlendDesc desc;
		desc.srcColorBlend = BlendMode::SourceAlpha;
		desc.srcAlphaBlend = BlendMode::SourceAlpha;
		desc.dstColorBlend = BlendMode::InverseSourceAlpha;
		desc.dstAlphaBlend = BlendMode::InverseSourceAlpha;
		_defaultBlendNonPremul = createBlendState(desc);
	}
	return _defaultBlendNonPremul;
}

std::shared_ptr<IBlendState> DXGraphicsDevice::getDefaultBlendOpaque() {
	if( nullptr == _defaultBlendOpaque ) {
		BlendDesc desc;
		desc.srcColorBlend = BlendMode::One;
		desc.srcAlphaBlend = BlendMode::One;
		desc.dstColorBlend = BlendMode::Zero;
		desc.dstAlphaBlend = BlendMode::Zero;
		_defaultBlendOpaque = createBlendState(desc);
	}
	return _defaultBlendOpaque;
}

std::shared_ptr<IRasterizerState> DXGraphicsDevice::getDefaultRasterNone() {
	if( nullptr == _defaultRasterNone ) {
		RasterizerDesc desc;
		desc.cullMode = CullMode::None;
		_defaultRasterNone = createRasterizerState(desc);
	}
	return _defaultRasterNone;
}

std::shared_ptr<IRasterizerState> DXGraphicsDevice::getDefaultRasterClockwise() {
	if( nullptr == _defaultRasterClockwise ) {
		RasterizerDesc desc;
		desc.cullMode = CullMode::Clockwise;
		_defaultRasterClockwise = createRasterizerState(desc);
	}
	return _defaultRasterClockwise;
}

std::shared_ptr<IRasterizerState> DXGraphicsDevice::getDefaultRasterCounterClockwise() {
	if( nullptr == _defaultRasterCounterClockwise ){
		RasterizerDesc desc;
		desc.cullMode = CullMode::CounterClockwise;
		_defaultRasterCounterClockwise = createRasterizerState(desc);
	}
	return _defaultRasterCounterClockwise;
}

std::shared_ptr<IDepthStencilState> DXGraphicsDevice::getDefaultDepthStencilDefault() {
	if( nullptr == _defaultDepthStencilDefault ) {
		DepthStencilDesc desc;
		desc.depthEnable = true;
		desc.depthWriteMask = true;
		_defaultDepthStencilDefault = createDepthStencilState(desc);
	}
	return _defaultDepthStencilDefault;
}

std::shared_ptr<IDepthStencilState> DXGraphicsDevice::getDefaultDepthStencilDepthRead() {
	if( nullptr == _defaultDepthStencilDepthRead ) {
		DepthStencilDesc desc;
		desc.depthEnable = true;
		desc.depthWriteMask = false;
		_defaultDepthStencilDepthRead = createDepthStencilState(desc);
	}
	return _defaultDepthStencilDepthRead;
}

std::shared_ptr<IDepthStencilState> DXGraphicsDevice::getDefaultDepthStencilNone() {
	if( nullptr == _defaultDepthStencilNone ) {
		DepthStencilDesc desc;
		desc.depthEnable = false;
		desc.depthWriteMask = false;
		_defaultDepthStencilNone = createDepthStencilState(desc);
	}
	return _defaultDepthStencilNone;
}

ID3D11Device* DXGraphicsDevice::getDevice() const {
	return _device;
}

ID3D11DeviceContext* DXGraphicsDevice::getContext() const {
	return _context;
}

bool DXGraphicsDevice::initDevice( unsigned int width, unsigned int height, HWND hwnd ) {
	HRESULT hr = S_OK;

	// todo: make these as parameters
	const bool IS_VSYNC_ENABLED = true;
	const UINT MSAA_SAMPLES = 1;

	// create a dxgi factory
	IDXGIFactory* factory = nullptr;
	if( FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory)) ) {
		return false;
	}

	// disable alt+enter
	factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);

	// use factory to create an adapter for primary gpu
	IDXGIAdapter* adapter = nullptr;
	if( FAILED(factory->EnumAdapters(0, &adapter)) ) {
		factory->Release();
		return false;
	}

	// enumerate adapter output (monitor)
	IDXGIOutput* adapterOutput = nullptr;
	if( FAILED(adapter->EnumOutputs(0, &adapterOutput)) ) {
		adapter->Release();
		factory->Release();
		return false;
	}

	// get the number of modes that fit the backbuffer display format
	const DXGI_FORMAT backbufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	UINT numDisplayModes = 0;
	if( FAILED(adapterOutput->GetDisplayModeList(backbufferFormat, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, nullptr)) ) {
		adapterOutput->Release();
		adapter->Release();
		factory->Release();
		return false;
	}

	// get all of the display modes
	DXGI_MODE_DESC* displayModes = new DXGI_MODE_DESC[numDisplayModes];
	if( FAILED(adapterOutput->GetDisplayModeList(backbufferFormat, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, displayModes)) ) {
		delete[] displayModes;
		return false;
	}

	// find a display mode that matches the screen width and height, and store its refresh rate information
	UINT refreshRateNumerator = 0;
	UINT refreshRateDenominator = 1;
	for( unsigned int i = 0; i < numDisplayModes; ++i ) {
		if( (width == displayModes[i].Width) && (height == displayModes[i].Height) ) {
			refreshRateNumerator = displayModes[i].RefreshRate.Numerator;
			refreshRateDenominator = displayModes[i].RefreshRate.Denominator;
			//break;
		}
	}

	// delete display modes array
	delete[] displayModes;
	displayModes = nullptr;

	// get the adapter description
	DXGI_ADAPTER_DESC adapterDesc;
	if( FAILED(adapter->GetDesc(&adapterDesc)) ) {
		return false;
	}

	// get gpu information
	//const int gpuMemory = static_cast<int>(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	_gpuName = strutil::wstr2str_utf8(adapterDesc.Description);
	_apiInfo = "DX11";

	// release adapter output, adapter, and factory
	adapterOutput->Release();
	adapterOutput = nullptr;
	adapter->Release();
	adapter = nullptr;
	factory->Release();
	factory = nullptr;

	// create and populate the swapchain description
	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(swapDesc));
	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Width = width;
	swapDesc.BufferDesc.Height = height;
	swapDesc.BufferDesc.Format = backbufferFormat;
	swapDesc.BufferDesc.RefreshRate.Numerator = (IS_VSYNC_ENABLED) ? refreshRateNumerator : 0;
	swapDesc.BufferDesc.RefreshRate.Denominator = (IS_VSYNC_ENABLED) ? refreshRateDenominator : 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = hwnd;
	swapDesc.SampleDesc.Count = MSAA_SAMPLES;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.Windowed = TRUE; // todo: fullscreen support
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Flags = 0;

	// setup device creation flags
	UINT createDeviceFlags = 0;
	#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	// create device, context, and swapchain
	const D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	if( FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, &featureLevel, 1, D3D11_SDK_VERSION, &swapDesc, &_swapchain, &_device, nullptr, &_context)) ) {
		return false;
	}

	if( !createBackbufferRtv() ) {
		return false;
	}

	if( !createDepthStencilView() ) {
		return false;
	}

	// set the backbuffer as the current render target with the depth stencil view, too
	_context->OMSetRenderTargets(1, &_backbuffer, _depthStencilView);

	// setup the viewport
	setViewport(Viewport(0, 0, width, height));

	// holy crap that was long
	return true;
}

bool DXGraphicsDevice::createBackbufferRtv() {
	// get a pointer to the backbuffer texture
	ID3D11Texture2D* backbufferPtr = nullptr;
	if( FAILED(_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backbufferPtr)) ) {
		return false;
	}
		
	// create a render target view from the backbuffer texture
	if( FAILED(_device->CreateRenderTargetView(backbufferPtr, nullptr, &_backbuffer)) ) {
		backbufferPtr->Release();
		return false;
	}

	// clean up backbuffer texture pointer
	backbufferPtr->Release();
	backbufferPtr = nullptr;

	return true;
}

bool DXGraphicsDevice::createDepthStencilView() {
	// create the depth stencil texture
	D3D11_TEXTURE2D_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(depthDesc));
	depthDesc.Width = _defaultWidth;
	depthDesc.Height = _defaultHeight;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;
	if( FAILED(_device->CreateTexture2D(&depthDesc, nullptr, &_depthStencil)) ) {
		_depthStencil = nullptr;
		return false;
	}

	// create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format = depthDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	if( FAILED(_device->CreateDepthStencilView(_depthStencil, &dsvDesc, &_depthStencilView)) ) {
		_depthStencil->Release();
		_depthStencil = nullptr;
		_depthStencilView = nullptr;
		return false;
	}

	return true;
}

namespace ciri {
std::shared_ptr<IGraphicsDevice> createGraphicsDevice() {
	return std::shared_ptr<IGraphicsDevice>(new DXGraphicsDevice());
}
}