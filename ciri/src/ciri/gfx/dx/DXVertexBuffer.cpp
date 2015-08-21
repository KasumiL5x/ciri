#include <ciri/gfx/dx/DXVertexBuffer.hpp>
#include <ciri/gfx/dx/DXGraphicsDevice.hpp>

namespace ciri {
	DXVertexBuffer::DXVertexBuffer( DXGraphicsDevice* device )
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

	err::ErrorCode DXVertexBuffer::set( void* vertices, int vertexStride, int vertexCount, bool dynamic ) {
		if( nullptr == vertices || vertexStride <= 0 || vertexCount <= 0 ) {
			return err::CIRI_INVALID_ARGUMENT;
		}

		if( _vertexBuffer != nullptr ) {
			// cannot update a static vertex buffer
			if( !_isDynamic ) {
				return err::CIRI_STATIC_BUFFER_AS_DYNAMIC;
			}

			// for now, size must be the same as the original data
			if( vertexStride != _vertexStride || vertexCount != _vertexCount ) {
				return err::CIRI_NOT_IMPLEMENTED; // todo
			}

			D3D11_MAPPED_SUBRESOURCE map;
			ZeroMemory(&map, sizeof(map));
			if( FAILED(_device->getContext()->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map)) ) {
				return err::CIRI_BUFFER_MAP_FAILED;
			}
			memcpy(map.pData, vertices, (vertexStride * vertexCount));
			_device->getContext()->Unmap(_vertexBuffer, 0);
			return err::CIRI_OK;
		}

		_vertexStride = vertexStride;
		_vertexCount = vertexCount;
		_isDynamic = dynamic;

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth = vertexStride * vertexCount;
		desc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = vertices;
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		if( FAILED(_device->getDevice()->CreateBuffer(&desc, &data, &_vertexBuffer)) ) {
			destroy();
			return err::CIRI_UNKNOWN_ERROR; // todo
		}

		return err::CIRI_OK;
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
} // ciri