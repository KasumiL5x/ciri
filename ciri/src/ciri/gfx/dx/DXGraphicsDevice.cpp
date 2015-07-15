#include <ciri/gfx/dx/DXGraphicsDevice.hpp>
#include <ciri/wnd/Window.hpp>
#include <DirectXColors.h>

namespace ciri {
	DXGraphicsDevice::DXGraphicsDevice()
		: IGraphicsDevice(), _driverType(D3D_DRIVER_TYPE_NULL), _featureLevel(D3D_FEATURE_LEVEL_11_0), _device(nullptr),
			_device1(nullptr), _immediateContext(nullptr), _immediateContext1(nullptr), _swapchain(nullptr),
			_swapchain1(nullptr), _renderTargetView(nullptr), _activeShader(nullptr), _activeVertexBuffer(nullptr) {
	}

	DXGraphicsDevice::~DXGraphicsDevice() {
		destroy();
	}

	bool DXGraphicsDevice::create( Window* window ) {
		cc::Vec2ui windowSize = window->getSize();
		if( !initDevice(windowSize.x, windowSize.y, window->getHandle()) ) {
			destroy();
			return false;
		}

		return true;
	}

	void DXGraphicsDevice::destroy() {
		// destroy all vertex buffers
		for( unsigned int i = 0; i < _vertexBuffers.size(); ++i ) {
			if( _vertexBuffers[i] != nullptr ) {
				_vertexBuffers[i]->destroy();
				delete _vertexBuffers[i];
				_vertexBuffers[i] = nullptr;
			}
		}
		_vertexBuffers.clear();

		// destroy all shaders
		for( unsigned int i = 0; i < _shaders.size(); ++i ) {
			if( _shaders[i] != nullptr ) {
				_shaders[i]->destroy();
				delete _shaders[i];
				_shaders[i] = nullptr;
			}
		}
		_shaders.clear();

		if( _immediateContext )  { _immediateContext->ClearState(); _immediateContext = nullptr; }
		if( _renderTargetView )  { _renderTargetView->Release(); _renderTargetView = nullptr; }
		if( _swapchain1 )        { _swapchain1->Release(); _swapchain1 = nullptr; }
		if( _swapchain )         { _swapchain->Release(); _swapchain = nullptr; }
		if( _immediateContext1 ) { _immediateContext1->Release(); _immediateContext1 = nullptr; }
		if( _immediateContext )  { _immediateContext->Release(); _immediateContext = nullptr; }
		if( _device1 )           { _device1->Release(); _device1 = nullptr; }
		if( _device )            { _device->Release(); _device = nullptr; }
	}

	void DXGraphicsDevice::present() {
		_swapchain->Present(0, 0);
	}

	IShader* DXGraphicsDevice::createShader() {
		DXShader* shader = new DXShader(this);
		_shaders.push_back(shader);
		return shader;
	}

	void DXGraphicsDevice::applyShader( IShader* shader ) {
		if( !shader->isValid() ) {
			_activeShader = nullptr;
			return;
		}

		DXShader* dxShader = reinterpret_cast<DXShader*>(shader);

		ID3D11VertexShader* vs = dxShader->getVertexShader();
		if( vs != nullptr ) {
			_immediateContext->VSSetShader(vs, nullptr, 0);
		}

		ID3D11GeometryShader* gs = dxShader->getGeometryShader();
		if( gs != nullptr ) {
			_immediateContext->GSSetShader(gs, nullptr, 0);
		}

		ID3D11PixelShader* ps = dxShader->getPixelShader();
		if( ps != nullptr ) {
			_immediateContext->PSSetShader(ps, nullptr, 0);
		}

		ID3D11InputLayout* il = dxShader->getInputLayout();
		if( il != nullptr ) {
			_immediateContext->IASetInputLayout(il);
		}

		_activeShader = dxShader;
	}

	IVertexBuffer* DXGraphicsDevice::createVertexBuffer() {
		DXVertexBuffer* buffer = new DXVertexBuffer(this);
		_vertexBuffers.push_back(buffer);
		return buffer;
	}

	void DXGraphicsDevice::setVertexBuffer( IVertexBuffer* buffer ) {
		// gl cannot set parameters with no active shader; dx can, but let's stay consistent
		if( nullptr == _activeShader ) {
			_activeVertexBuffer = nullptr;
			return;
		}

		DXVertexBuffer* dxBuffer = reinterpret_cast<DXVertexBuffer*>(buffer);
		UINT stride = buffer->getStride();
		UINT offset = 0;
		ID3D11Buffer* vb = dxBuffer->getVertexBuffer();
		_immediateContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

		_activeVertexBuffer = dxBuffer;
	}

	void DXGraphicsDevice::drawArrays( PrimitiveTopology::Type topology, int vertexCount, int startIndex ) {
		// cannot draw with no active shader
		if( nullptr == _activeShader ) {
			return;
		}

		// cannot draw with no active vertex buffer
		if( nullptr == _activeVertexBuffer ) {
			return;
		}

		// vertex count must be greater than 0!
		if( vertexCount <= 0 ) {
			return;
		}

		// start index must be positive and less than vertex count-1
		if( startIndex <= 0 || startIndex >= vertexCount ) {
			return;
		}

		switch( topology ) {
			case PrimitiveTopology::PointList: {
				_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			}

			case PrimitiveTopology::LineList: {
				_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			}

			case PrimitiveTopology::LineStrip: {
				_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
			}

			case PrimitiveTopology::TriangleList: {
				_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			}

			case PrimitiveTopology::TriangleStrip: {
				_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			}

			default: {
				break;
			}
		}

		_immediateContext->Draw(vertexCount, startIndex);
	}

	void DXGraphicsDevice::clear() {
		_immediateContext->ClearRenderTargetView(_renderTargetView, DirectX::Colors::CornflowerBlue);
	}

	ID3D11Device* DXGraphicsDevice::getDevice() const {
		return _device;
	}

	ID3D11DeviceContext* DXGraphicsDevice::getContext() const {
		return _immediateContext;
	}

	bool DXGraphicsDevice::initDevice( unsigned int width, unsigned int height, HWND hwnd ) {
		HRESULT hr = S_OK;

		UINT createDeviceFlags = 0;
		#ifdef _DEBUG
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		#endif

		D3D_DRIVER_TYPE driverTypes[] = {
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		for( UINT driverTypeIdx = 0; driverTypeIdx < numDriverTypes; ++driverTypeIdx ) {
			_driverType = driverTypes[driverTypeIdx];
			hr = D3D11CreateDevice(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &_device, &_featureLevel, &_immediateContext);
			if( E_INVALIDARG == hr ) {
				// dx 11.0 platforms don't recognize d3d_feature_leelv_11_1, so retry without it
				hr = D3D11CreateDevice(nullptr, _driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels-1, D3D11_SDK_VERSION, &_device, &_featureLevel, &_immediateContext);
			}
			if( SUCCEEDED(hr) ) {
				break;
			}
		}
		if( FAILED(hr) ) {
			return false;
		}

		// obtain dxgi factory from device (because we used nullptr for adapter above)
		IDXGIFactory1* dxgiFactory = nullptr;
		{
			IDXGIDevice* dxgiDevice = nullptr;
			hr = _device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
			if( SUCCEEDED(hr) ) {
				IDXGIAdapter* adapter = nullptr;
				hr = dxgiDevice->GetAdapter(&adapter);
				if( SUCCEEDED(hr) ) {
					hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
					adapter->Release();
				}
				dxgiDevice->Release();
			}
		}
		if( FAILED(hr) ) {
			return false;
		}

		// create swapchain
		IDXGIFactory2* dxgiFactory2 = nullptr;
		hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
		if( dxgiFactory2 ) {
			// dx 11.1 or later
			hr = _device->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&_device1));
			if( SUCCEEDED(hr) ) {
				_immediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&_immediateContext1));
			}

			DXGI_SWAP_CHAIN_DESC1 desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Width = width;
			desc.Height = height;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			desc.BufferCount = 1;

			hr = dxgiFactory2->CreateSwapChainForHwnd(_device, hwnd, &desc, nullptr, nullptr, &_swapchain1);
			if( SUCCEEDED(hr) ) {
				hr = _swapchain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&_swapchain));
			}

			dxgiFactory2->Release();
		} else {
			// dx 11.0
			DXGI_SWAP_CHAIN_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.BufferCount = 1;
			desc.BufferDesc.Width = width;
			desc.BufferDesc.Height = height;
			desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.BufferDesc.RefreshRate.Numerator = 60;
			desc.BufferDesc.RefreshRate.Denominator = 1;
			desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			desc.OutputWindow = hwnd;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Windowed = TRUE;

			hr = dxgiFactory->CreateSwapChain(_device, &desc, &_swapchain);
		}

		// disable alt+enter to toggle fullscreen
		dxgiFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);

		dxgiFactory->Release();

		if( FAILED(hr) ) {
			return false;
		}

		// create render target view
		ID3D11Texture2D* backbuffer = nullptr;
		hr = _swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backbuffer));
		if( FAILED(hr) ) {
			return false;
		}
		hr = _device->CreateRenderTargetView(backbuffer, nullptr, &_renderTargetView);
		backbuffer->Release();
		if( FAILED(hr) ) {
			return false;
		}
		_immediateContext->OMSetRenderTargets(1, &_renderTargetView, nullptr);

		// setup the viewport
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)width;
		vp.Height = (FLOAT)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		_immediateContext->RSSetViewports(1, &vp);

		return true;
	}
} // ciri