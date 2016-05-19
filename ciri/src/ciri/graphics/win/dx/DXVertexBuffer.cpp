#include <ciri/graphics/win/dx/DXVertexBuffer.hpp>
#include <ciri/graphics/win/dx/DXGraphicsDevice.hpp>

using namespace ciri::graphics;
using namespace ciri::core;

DXVertexBuffer::DXVertexBuffer( const std::shared_ptr<DXGraphicsDevice>& device )
	: IVertexBuffer(), _device(device), _vertexBuffer(nullptr), _vertexStride(0), _vertexCount(0), _isDynamic(false) {
}

DXVertexBuffer::~DXVertexBuffer() {
	destroy();
}

void DXVertexBuffer::destroy() {
	if( _vertexBuffer != nullptr ) {
		_vertexBuffer->Release();
		_vertexBuffer = nullptr;
	}
}

ErrorCode DXVertexBuffer::set( void* vertices, int vertexStride, int vertexCount, bool dynamic ) {
	if( nullptr == vertices || vertexStride <= 0 || vertexCount <= 0 ) {
		return ErrorCode::CIRI_INVALID_ARGUMENT;
	}

	// update if already valid
	if( _vertexBuffer != nullptr ) {
		// cannot update a static vertex buffer
		if( !_isDynamic ) {
			return ErrorCode::CIRI_STATIC_BUFFER_AS_DYNAMIC;
		}

		// do not let the vertex stride change regardless until i decide how to handle it
		if( _vertexStride != vertexStride ) {
			return ErrorCode::CIRI_NOT_IMPLEMENTED;
		}

		// if the new data is larger...
		if( (vertexStride * vertexCount) > (_vertexStride * _vertexCount) ) {
			destroy();
			return createBuffer(vertices, vertexStride, vertexCount, true);
		}

		return updateBuffer(vertices, vertexStride, vertexCount);
	}

	return createBuffer(vertices, vertexStride, vertexCount, dynamic);
}

int DXVertexBuffer::getStride() const {
	return _vertexStride;
}

int DXVertexBuffer::getVertexCount() {
	return _vertexCount;
}

ID3D11Buffer* DXVertexBuffer::getVertexBuffer() const {
	return _vertexBuffer;
}

ErrorCode DXVertexBuffer::createBuffer( void* vertices, int vertexStride, int vertexCount, bool dynamic ) {
	// description of the vertex buffer
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ByteWidth = vertexStride * vertexCount;
	desc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	// data to initialize the vertex buffer with
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = vertices;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	// create actual vertex buffer with initial data
	if( FAILED(_device->getDevice()->CreateBuffer(&desc, &data, &_vertexBuffer)) ) {
		destroy();
		return ErrorCode::CIRI_BUFFER_CREATION_FAILED;
	}

	// store settings upon successful buffer creation
	_vertexStride = vertexStride;
	_vertexCount = vertexCount;
	_isDynamic = dynamic;

	return ErrorCode::CIRI_OK;
}

ErrorCode DXVertexBuffer::updateBuffer( void* vertices, int vertexStride, int vertexCount ) {
		// map buffer to get pointer
		D3D11_MAPPED_SUBRESOURCE map;
		ZeroMemory(&map, sizeof(map));
		if( FAILED(_device->getContext()->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map)) ) {
			return ErrorCode::CIRI_BUFFER_MAP_FAILED;
		}

		// copy data and unmap
		memcpy(map.pData, vertices, (vertexStride * vertexCount));
		_device->getContext()->Unmap(_vertexBuffer, 0);

		return ErrorCode::CIRI_OK;
}