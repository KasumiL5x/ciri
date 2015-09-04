#ifndef __ciri_dxgraphicsdevice__
#define __ciri_dxgraphicsdevice__

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include <vector>
#include <string>
#include <d3d11.h>
#include "../IGraphicsDevice.hpp"
#include "DXShader.hpp"
#include "DXVertexBuffer.hpp"
#include "DXIndexBuffer.hpp"
#include "DXConstantBuffer.hpp"
#include "DXTexture2D.hpp"
#include "DXSamplerState.hpp"
#include "DXRenderTarget2D.hpp"
#include "DXRasterizerState.hpp"
#include "DXDepthStencilState.hpp"
#include "DXBlendState.hpp"
#include "DXTextureCube.hpp"

namespace ciri {
	class DXGraphicsDevice : public IGraphicsDevice {
	public:
		DXGraphicsDevice();
		virtual ~DXGraphicsDevice();

		virtual bool create( Window* window );
		virtual void destroy();
		virtual void present();

		virtual IShader* createShader();
		virtual IVertexBuffer* createVertexBuffer();
		virtual IIndexBuffer* createIndexBuffer();
		virtual IConstantBuffer* createConstantBuffer();
		virtual ITexture2D* createTexture2D( int width, int height, TextureFormat::Format format, int flags, void* pixels=nullptr );
		virtual ITextureCube* createTextureCube( int width, int height, void* posx, void* negx, void* posy, void* negy, void* posz, void* negz ) override;
		virtual ISamplerState* createSamplerState( const SamplerDesc& desc );
		virtual IRenderTarget2D* createRenderTarget2D( int width, int height, TextureFormat::Format format );
		virtual IRasterizerState* createRasterizerState( const RasterizerDesc& desc );
		virtual IDepthStencilState* createDepthStencilState( const DepthStencilDesc& desc );
		virtual IBlendState* createBlendState( const BlendDesc& desc ) override;
		virtual void applyShader( IShader* shader );
		virtual void setVertexBuffer( IVertexBuffer* buffer );
		virtual void setIndexBuffer( IIndexBuffer* buffer );
		virtual void setTexture2D( int index, ITexture2D* texture, ShaderStage::Stage shaderStage );
		virtual void setTextureCube( int index, ITextureCube* texture, ShaderStage::Stage shaderStage ) override;
		virtual void setSamplerState( int index, ISamplerState* state, ShaderStage::Stage shaderStage );
		virtual void setBlendState( IBlendState* state ) override;
		virtual void drawArrays( PrimitiveTopology::Topology topology, int vertexCount, int startIndex );
		virtual void drawIndexed( PrimitiveTopology::Topology topology, int indexCount );
		virtual void setRenderTargets( IRenderTarget2D** renderTargets, int numRenderTargets );
		virtual void restoreDefaultRenderTargets();
		virtual err::ErrorCode resize();
		virtual void setClearColor( float r, float g, float b, float a );
		virtual void clear( int flags );
		virtual void setRasterizerState( IRasterizerState* state );
		virtual void setDepthStencilState( IDepthStencilState* state );
		virtual void setShaderExt( const char* ext );
		virtual const char* getShaderExt() const;
		virtual Window* getWindow() const;
		virtual const char* getGpuName() const;
		virtual const char* getApiInfo() const;
		virtual GraphicsApiType::Type getApiType() const override;

		ID3D11Device* getDevice() const;
		ID3D11DeviceContext* getContext() const;

	private:
		bool initDevice( unsigned int width, unsigned int height, HWND hwnd );
		bool createBackbufferRtv();
		bool createDepthStencilView();

	private:
		bool _isValid;
		Window* _window;
		//
		IDXGISwapChain* _swapchain;
		ID3D11Device* _device;
		ID3D11DeviceContext* _context;
		ID3D11RenderTargetView* _backbuffer;
		int _defaultWidth;
		int _defaultHeight;
		//
		float _clearColor[4];
		//
		std::vector<DXShader*> _shaders;
		std::vector<DXVertexBuffer*> _vertexBuffers;
		std::vector<DXIndexBuffer*> _indexBuffers;
		//
		DXShader* _activeShader;
		DXVertexBuffer* _activeVertexBuffer;
		DXIndexBuffer* _activeIndexBuffer;
		//
		std::vector<DXConstantBuffer*> _constantBuffers;
		//
		std::vector<DXTexture2D*> _texture2Ds;
		//
		std::vector<DXSamplerState*> _samplers;
		//
		std::vector<DXRenderTarget2D*> _renderTarget2Ds;
		//
		std::vector<DXRasterizerState*> _rasterizerStates;
		IRasterizerState* _activeRasterizerState;
		//
		std::vector<DXDepthStencilState*> _depthStencilStates;
		IDepthStencilState* _activeDepthStencilState;
		ID3D11Texture2D* _depthStencil;
		ID3D11DepthStencilView* _depthStencilView;
		ID3D11RenderTargetView* _activeRenderTargets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
		//
		std::string _shaderExt;
		//
		std::string _gpuName;
		std::string _apiInfo;
		//
		std::vector<DXBlendState*> _blendStates;
		//
		std::vector<DXTextureCube*> _textureCubes;
	};
} // ciri

#endif /* __ciri_dxgraphicsdevice__ */
