#ifndef __ciri_iconstantbuffer__
#define __ciri_iconstantbuffer__

#include <ciri/ErrorCodes.hpp>

namespace ciri {
	class IConstantBuffer {
	public:
		IConstantBuffer() {
		}

		virtual ~IConstantBuffer() {
		}

		/**
		 * Sets or updates the contents of the constant buffer.
		 * @param dataSize Size in bytes of the data.
		 * @param data     Pointer to the data to set.
		 * @returns ErrorCode indicating success or failure.
		 */
		virtual err::ErrorCode setData( int dataSize, void* data )=0;

		/**
		 * Uninitializes the constant buffer.
		 */
		virtual void destroy()=0;
	};
} // ciri

#endif /* __ciri_iconstantbuffer__ */
