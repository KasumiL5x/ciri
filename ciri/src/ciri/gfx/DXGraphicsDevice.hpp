#ifndef __ciri_dxgraphicsdevice__
#define __ciri_dxgraphicsdevice__

#pragma comment(lib, "d3d11.lib")

#include <d3d11_1.h>
#include "IGraphicsDevice.hpp"

namespace ciri {
	class DXGraphicsDevice : public IGraphicsDevice {
	public:
		DXGraphicsDevice();
		virtual ~DXGraphicsDevice();

		virtual bool create( Window* window );
		virtual void destroy();
		virtual void present();

	private:
		bool initDevice( unsigned int width, unsigned int height, HWND hwnd );

	private:
		D3D_DRIVER_TYPE _driverType;
		D3D_FEATURE_LEVEL _featureLevel;
		ID3D11Device* _device;
		ID3D11Device1* _device1;
		ID3D11DeviceContext* _immediateContext;
		ID3D11DeviceContext1* _immediateContext1;
		IDXGISwapChain* _swapchain;
		IDXGISwapChain1* _swapchain1;
		ID3D11RenderTargetView* _renderTargetView;
	};
} // ciri

#endif /* __ciri_dxgraphicsdevice__ */