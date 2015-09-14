#ifndef __ciri_gfx_dxgraphicsdevice__
#define __ciri_gfx_dxgraphicsdevice__

#include <memory>
#include <vector>
#include <string>
#include <d3d11.h>
#include <ciri/gfx/IGraphicsDevice.hpp>
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
	class DXGraphicsDevice : public IGraphicsDevice, public std::enable_shared_from_this<DXGraphicsDevice> {
	public:
		DXGraphicsDevice();
		virtual ~DXGraphicsDevice();

		virtual bool create( const std::shared_ptr<IWindow>& window ) override;
		virtual void destroy() override;
		virtual void present() override;

		virtual std::shared_ptr<IShader> createShader() override;
		virtual std::shared_ptr<IVertexBuffer> createVertexBuffer() override;
		virtual std::shared_ptr<IIndexBuffer> createIndexBuffer() override;
		virtual std::shared_ptr<IConstantBuffer> createConstantBuffer() override;
		virtual std::shared_ptr<ITexture2D> createTexture2D( int width, int height, TextureFormat::Format format, int flags, void* pixels=nullptr ) override;
		virtual std::shared_ptr<ITextureCube> createTextureCube( int width, int height, void* posx, void* negx, void* posy, void* negy, void* posz, void* negz ) override;
		virtual std::shared_ptr<ISamplerState> createSamplerState( const SamplerDesc& desc ) override;
		virtual std::shared_ptr<IRenderTarget2D> createRenderTarget2D( int width, int height, TextureFormat::Format format ) override;
		virtual std::shared_ptr<IRasterizerState> createRasterizerState( const RasterizerDesc& desc ) override;
		virtual std::shared_ptr<IDepthStencilState> createDepthStencilState( const DepthStencilDesc& desc ) override;
		virtual std::shared_ptr<IBlendState> createBlendState( const BlendDesc& desc ) override;
		virtual void applyShader( const std::shared_ptr<IShader>& shader ) override;
		virtual void setVertexBuffer( const std::shared_ptr<IVertexBuffer>& buffer ) override;
		virtual void setIndexBuffer( const std::shared_ptr<IIndexBuffer>& buffer ) override;
		virtual void setTexture2D( int index, const std::shared_ptr<ITexture2D>& texture, ShaderStage::Stage shaderStage ) override;
		virtual void setTextureCube( int index, const std::shared_ptr<ITextureCube>& texture, ShaderStage::Stage shaderStage ) override;
		virtual void setSamplerState( int index, const std::shared_ptr<ISamplerState>& state, ShaderStage::Stage shaderStage ) override;
		virtual void setBlendState( const std::shared_ptr<IBlendState>& state ) override;
		virtual void drawArrays( PrimitiveTopology topology, int vertexCount, int startIndex ) override;
		virtual void drawIndexed( PrimitiveTopology topology, int indexCount ) override;
		virtual void setRenderTargets( IRenderTarget2D** renderTargets, int numRenderTargets ) override;
		virtual void restoreDefaultRenderTargets() override;
		virtual ErrorCode resize() override;
		virtual void setClearColor( float r, float g, float b, float a ) override;
		virtual void clear( int flags ) override;
		virtual void setRasterizerState( const std::shared_ptr<IRasterizerState>& state ) override;
		virtual void setDepthStencilState( const std::shared_ptr<IDepthStencilState>& state ) override;
		virtual void setShaderExt( const char* ext ) override;
		virtual const char* getShaderExt() const override;
		virtual std::shared_ptr<IWindow> getWindow() const override;
		virtual const char* getGpuName() const override;
		virtual const char* getApiInfo() const override;
		virtual GraphicsApiType getApiType() const override;

		ID3D11Device* getDevice() const;
		ID3D11DeviceContext* getContext() const;

	private:
		bool initDevice( unsigned int width, unsigned int height, HWND hwnd );
		bool createBackbufferRtv();
		bool createDepthStencilView();

	private:
		bool _isValid;
		std::shared_ptr<IWindow> _window;
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
		std::vector<std::shared_ptr<DXShader>> _shaders;
		std::vector<std::shared_ptr<DXVertexBuffer>> _vertexBuffers;
		std::vector<std::shared_ptr<DXIndexBuffer>> _indexBuffers;
		//
		std::weak_ptr<DXShader> _activeShader;
		std::weak_ptr<DXVertexBuffer> _activeVertexBuffer;
		std::weak_ptr<DXIndexBuffer> _activeIndexBuffer;
		//
		std::vector<std::shared_ptr<DXConstantBuffer>> _constantBuffers;
		//
		std::vector<std::shared_ptr<DXTexture2D>> _texture2Ds;
		//
		std::vector<std::shared_ptr<DXSamplerState>> _samplers;
		//
		std::vector<std::shared_ptr<DXRenderTarget2D>> _renderTarget2Ds;
		//
		std::vector<std::shared_ptr<DXRasterizerState>> _rasterizerStates;
		std::weak_ptr<DXRasterizerState> _activeRasterizerState;
		//
		std::vector<std::shared_ptr<DXDepthStencilState>> _depthStencilStates;
		std::weak_ptr<DXDepthStencilState> _activeDepthStencilState;
		ID3D11Texture2D* _depthStencil;
		ID3D11DepthStencilView* _depthStencilView;
		ID3D11RenderTargetView* _activeRenderTargets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
		//
		std::string _shaderExt;
		//
		std::string _gpuName;
		std::string _apiInfo;
		//
		std::vector<std::shared_ptr<DXBlendState>> _blendStates;
		//
		std::vector<std::shared_ptr<DXTextureCube>> _textureCubes;
	};
} // ciri

#endif /* __ciri_gfx_dxgraphicsdevice__ */
