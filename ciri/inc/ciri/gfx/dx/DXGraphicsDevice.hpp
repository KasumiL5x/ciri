#ifndef __ciri_dxgraphicsdevice__
#define __ciri_dxgraphicsdevice__

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include <vector>
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
		virtual ITexture2D* createTexture2D();
		virtual ISamplerState* createSamplerState( const SamplerDesc& desc );
		virtual IRenderTarget2D* createRenderTarget2D( int width, int height, TextureFormat::Type format );
		virtual IRasterizerState* createRasterizerState( const RasterizerDesc& desc );
		virtual IDepthStencilState* createDepthStencilState( const DepthStencilDesc& desc );

		virtual void applyShader( IShader* shader );

		virtual void setVertexBuffer( IVertexBuffer* buffer );
		virtual void setIndexBuffer( IIndexBuffer* buffer );

		virtual void setTexture2D( int index, ITexture2D* texture, ShaderStage::Stage shaderStage );
		virtual void setSamplerState( int index, ISamplerState* state, ShaderStage::Stage shaderStage );

		virtual void drawArrays( PrimitiveTopology::Type topology, int vertexCount, int startIndex );
		virtual void drawIndexed( PrimitiveTopology::Type topology, int indexCount );

		virtual void setRenderTargets( IRenderTarget2D** renderTargets, int numRenderTargets );
		virtual void restoreDefaultRenderTargets();
		virtual void clear( ClearFlags::Flags flags, float* color );

		virtual void setRasterizerState( IRasterizerState* state );
		virtual void setDepthStencilState( IDepthStencilState* state );

		ID3D11Device* getDevice() const;
		ID3D11DeviceContext* getContext() const;

	private:
		bool initDevice( unsigned int width, unsigned int height, HWND hwnd );
		D3D_PRIMITIVE_TOPOLOGY convertTopology( PrimitiveTopology::Type topology ) const;

	private:
		IDXGISwapChain* _swapchain;
		ID3D11Device* _device;
		ID3D11DeviceContext* _context;
		ID3D11RenderTargetView* _backbuffer;
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
	};
} // ciri

#endif /* __ciri_dxgraphicsdevice__ */
