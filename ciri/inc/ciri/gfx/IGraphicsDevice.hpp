#ifndef __ciri_igraphicsdevice__
#define __ciri_igraphicsdevice__

#include "PrimitiveTopology.hpp"

namespace ciri {
	class Window;
	class IShader;
	class IVertexBuffer;

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

		virtual void drawArrays( PrimitiveTopology::Type topology, int vertexCount, int startIndex )=0;

		virtual void clear()=0;
	};
} // ciri

#endif /* __ciri_igraphicsdevice__ */