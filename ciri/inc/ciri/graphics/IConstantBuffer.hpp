#ifndef __ciri_graphics_IConstantBuffer__
#define __ciri_graphics_IConstantBuffer__

#include <ciri/core/ErrorCodes.hpp>

namespace ciri {

class IConstantBuffer {
protected:
	IConstantBuffer() {
	}

public:
	virtual ~IConstantBuffer() {
	}

	/**
		* Sets or updates the contents of the constant buffer.
		* @param dataSize Size in bytes of the data.
		* @param data     Pointer to the data to set.
		* @returns ErrorCode indicating success or failure.
		*/
	virtual ErrorCode setData( int dataSize, void* data )=0;

	/**
		* Uninitializes the constant buffer.
		*/
	virtual void destroy()=0;
};

}

#endif