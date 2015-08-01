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
#include"IRenderTarget2D.hpp"

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
		virtual ITexture2D* createTexture2D()=0;
		virtual ISamplerState* createSamplerState( const SamplerDesc& desc )=0;
		virtual IRenderTarget2D* createRenderTarget2D( int width, int height, TextureFormat::Type format )=0;

		virtual void applyShader( IShader* shader )=0;

		virtual void setVertexBuffer( IVertexBuffer* buffer )=0;
		virtual void setIndexBuffer( IIndexBuffer* buffer )=0;

		virtual void setTexture2D( int index, ITexture2D* texture, ShaderStage::Stage shaderStage )=0;
		virtual void setSamplerState( int index, ISamplerState* state, ShaderStage::Stage shaderStage )=0;

		virtual void drawArrays( PrimitiveTopology::Type topology, int vertexCount, int startIndex )=0;
		virtual void drawIndexed( PrimitiveTopology::Type topology, int indexCount )=0;

		virtual void setRenderTargets( IRenderTarget2D** renderTargets, int numRenderTargets )=0;
		virtual void restoreDefaultRenderTargets()=0;
		virtual void clear( ClearFlags::Flags flags, float* color=nullptr )=0;
	};
} // ciri

#endif /* __ciri_igraphicsdevice__ */