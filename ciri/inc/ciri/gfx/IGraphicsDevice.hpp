#ifndef __ciri_igraphicsdevice__
#define __ciri_igraphicsdevice__

#include "ShaderStage.hpp"
#include "PrimitiveTopology.hpp"
#include "ISamplerState.hpp"

namespace ciri {
	class Window;
	class IShader;
	class IVertexBuffer;
	class IIndexBuffer;
	class IConstantBuffer;
	class ITexture2D;
	class IRenderTarget2D;

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

		virtual void applyShader( IShader* shader )=0;

		virtual IVertexBuffer* createVertexBuffer()=0;

		virtual void setVertexBuffer( IVertexBuffer* buffer )=0;

		virtual IIndexBuffer* createIndexBuffer()=0;

		virtual void setIndexBuffer( IIndexBuffer* buffer )=0;

		virtual void drawArrays( PrimitiveTopology::Type topology, int vertexCount, int startIndex )=0;

		virtual void drawIndexed( PrimitiveTopology::Type topology, int indexCount )=0;

		virtual void clear()=0;

		virtual IConstantBuffer* createConstantBuffer()=0;

		virtual ITexture2D* createTexture2D()=0;
		virtual void setTexture2D( int index, ITexture2D* texture, ShaderStage::Stage shaderStage )=0;

		virtual ISamplerState* createSamplerState( const SamplerDesc& desc )=0;
		virtual void setSamplerState( int index, ISamplerState* state, ShaderStage::Stage shaderStage )=0;

		virtual IRenderTarget2D* createRenderTarget2D( int width, int height )=0;
	};
} // ciri

#endif /* __ciri_igraphicsdevice__ */