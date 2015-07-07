#ifndef __ciri_dxgraphicsdevice_win__
#define __ciri_dxgraphicsdevice_win__

#pragma comment(lib, "d3d11.lib")

#include <d3d11_1.h>

namespace ciri {
	class Window;

	class DXGraphicsDevice_ps {
	public:
		DXGraphicsDevice_ps();
		~DXGraphicsDevice_ps();

		bool create( Window* window );
		void destroy();
		void present();

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

#endif /* __ciri_dxgraphicsdevice_win__ */