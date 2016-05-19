#ifndef __ciri_graphics_IVertexBuffer__
#define __ciri_graphics_IVertexBuffer__

#include <ciri/core/ErrorCodes.hpp>

namespace ciri {

class IVertexBuffer {
protected:
	IVertexBuffer() {
	}

public:
	virtual ~IVertexBuffer() {
	}

	/**
		* Sets or updates the contents of the vertex buffer.
		* @param vertices     Pointer to vertex array.
		* @param vertexStride Size in bytes of a single vertex.
		* @param vertexCount  Total number of vertices in the array.
		* @param dynamic      True if the buffer is dynamic; i.e. will be updated.
		* @returns ErrorCode indicating success or failure.
		*/
	virtual ErrorCode set( void* vertices, int vertexStride, int vertexCount, bool dynamic )=0;

	/**
		* Uninitializes the vertex buffer.
		*/
	virtual void destroy()=0;

	/**
		* Gets the size of a single vertex in bytes.
		* @return Size of a single vertex in bytes provided when data was set.
		*/
	virtual int getStride() const=0;

	/**
		* Gets the total number of vertices.
		* @return Total number of vertices provided when data was set.
		*/
	virtual int getVertexCount()=0;
};

}

#endif