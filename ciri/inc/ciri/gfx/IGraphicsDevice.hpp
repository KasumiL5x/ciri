#ifndef __ciri_igraphicsdevice__
#define __ciri_igraphicsdevice__

#include "PrimitiveTopology.hpp"
#include "ISamplerState.hpp"

namespace ciri {
	class Window;
	class IShader;
	class IVertexBuffer;
	class IIndexBuffer;
	class IConstantBuffer;
	class ITexture2D;

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

		virtual ISamplerState* createSamplerState( const SamplerDesc& desc )=0;
	};
} // ciri

#endif /* __ciri_igraphicsdevice__ */