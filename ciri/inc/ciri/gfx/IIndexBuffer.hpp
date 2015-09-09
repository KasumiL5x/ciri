#ifndef __ciri_iindexbuffer__
#define __ciri_iindexbuffer__

#include "../core/ErrorCodes.hpp"

namespace ciri {
	class IIndexBuffer {
	public:
		IIndexBuffer() {
		}
		virtual ~IIndexBuffer() {
		}

		/**
		 * Sets or updates the contents of the index buffer.
		 * @param indices    Pointer to index array.
		 * @param indexCount Total number of indices in the array.
		 * @param dynamic    True if the buffer is dynamic; i.e. will be updated.
		 * @returns err::ErrorCode indicating success or failure.
		 */
		virtual err::ErrorCode set( int* indices, int indexCount, bool dynamic )=0;

		/**
		 * Uninitializes the index buffer.
		 */
		virtual void destroy()=0;

		/**
		 * Gets the index count.
		 * @return Index count provided when data was set.
		 */
		virtual int getIndexCount() const=0;
	};
} // ciri

#endif /* __ciri_iindexbuffer__ */
