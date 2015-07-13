#ifndef __ciri_vertexbuffer__
#define __ciri_vertexbuffer__

namespace ciri {
	class IVertexBuffer {
	public:
		IVertexBuffer() {
		}
		virtual ~IVertexBuffer() {
		}

		virtual bool set( void* vertices, int vertexStride, int vertexCount, bool dynamic )=0;
		virtual void destroy()=0;
		virtual int getStride() const=0;
	};
} //ciri

#endif /* __ciri_vertexbuffer__ */