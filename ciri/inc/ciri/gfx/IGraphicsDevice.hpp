#ifndef __ciri_igraphicsdevice__
#define __ciri_igraphicsdevice__

#include "IShader.hpp"
#include "IVertexBuffer.hpp"
#include "IIndexBuffer.hpp"
#include "PrimitiveTopology.hpp"
#include "ClearFlags.hpp"
#include "IConstantBuffer.hpp"
#include "ITexture2D.hpp"
#include "ShaderStage.hpp"
#include "ISamplerState.hpp"
#include "TextureFormat.hpp"
#include "TextureFlags.hpp"
#include"IRenderTarget2D.hpp"
#include "IRasterizerState.hpp"
#include "IDepthStencilState.hpp"

namespace ciri {
	class Window;

	class IGraphicsDevice {
	public:
		IGraphicsDevice() {
		}

		virtual ~IGraphicsDevice() {
		}

		virtual bool create( Window* window )=0;
		virtual void destroy()=0;
		virtual void present()=0;

		virtual IShader* createShader()=0;
		virtual IVertexBuffer* createVertexBuffer()=0;
		virtual IIndexBuffer* createIndexBuffer()=0;
		virtual IConstantBuffer* createConstantBuffer()=0;
		virtual ITexture2D* createTexture2D( int width, int height, TextureFormat::Format format, int flags, void* pixels=nullptr )=0; // TextureFlags::Flags
		virtual ISamplerState* createSamplerState( const SamplerDesc& desc )=0;
		virtual IRenderTarget2D* createRenderTarget2D( int width, int height, TextureFormat::Format format )=0;
		virtual IRasterizerState* createRasterizerState( const RasterizerDesc& desc )=0;
		virtual IDepthStencilState* createDepthStencilState( const DepthStencilDesc& desc )=0;

		virtual void applyShader( IShader* shader )=0;

		virtual void setVertexBuffer( IVertexBuffer* buffer )=0;
		virtual void setIndexBuffer( IIndexBuffer* buffer )=0;

		virtual void setTexture2D( int index, ITexture2D* texture, ShaderStage::Stage shaderStage )=0;
		virtual void setSamplerState( int index, ISamplerState* state, ShaderStage::Stage shaderStage )=0;

		virtual void drawArrays( PrimitiveTopology::Topology topology, int vertexCount, int startIndex )=0;
		virtual void drawIndexed( PrimitiveTopology::Topology topology, int indexCount )=0;

		virtual void setRenderTargets( IRenderTarget2D** renderTargets, int numRenderTargets )=0;
		virtual void restoreDefaultRenderTargets()=0;
		virtual err::ErrorCode resizeDefaultRenderTargets( int width, int height )=0;
		virtual void setClearColor( float r, float g, float b, float a )=0;
		virtual void clear( int flags )=0; // ClearFlags::Flags

		virtual void setRasterizerState( IRasterizerState* state )=0;
		virtual void setDepthStencilState( IDepthStencilState* state )=0;

		virtual void setShaderExt( const char* ext )=0;
		virtual const char* getShaderExt() const=0;
	};
} // ciri

#endif /* __ciri_igraphicsdevice__ */