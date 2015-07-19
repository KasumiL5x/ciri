#ifndef __ciri_iindexbuffer__
#define __ciri_iindexbuffer__

namespace ciri {
	class IIndexBuffer {
	public:
		IIndexBuffer() {
		}
		virtual ~IIndexBuffer() {
		}

		virtual bool set( int* indices, int indexCount, bool dynamic )=0;
		virtual void destroy()=0;
	};
} // ciri

#endif /* __ciri_iindexbuffer__ */