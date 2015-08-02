#include <ciri/gfx/dx/DXIndexBuffer.hpp>
#include <ciri/gfx/dx/DXGraphicsDevice.hpp>

namespace ciri {
	DXIndexBuffer::DXIndexBuffer( DXGraphicsDevice* device )
		: IIndexBuffer(), _device(device), _indexBuffer(nullptr), _indexCount(0) {
	}

	DXIndexBuffer::~DXIndexBuffer() {
		destroy();
	}

	err::ErrorCode DXIndexBuffer::set( int* indices, int indexCount, bool dynamic ) {
		if( nullptr == indices || indexCount <= 0 ) {
			return err::CIRI_INVALID_ARGUMENT;
		}

		// todo: add remapping (updating) support
		if( _indexBuffer != nullptr ) {
			return err::CIRI_NOT_IMPLEMENTED;
		}

		_indexCount = indexCount;

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth = sizeof(int) * indexCount;
		desc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = indices;
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		if( FAILED(_device->getDevice()->CreateBuffer(&desc, &data, &_indexBuffer)) ) {
			destroy();
			return err::CIRI_UNKNOWN_ERROR;
		}

		return err::CIRI_OK;
	}

	void DXIndexBuffer::destroy() {
		if( _indexBuffer != nullptr ) {
			_indexBuffer->Release();
			_indexBuffer = nullptr;
		}
	}

	int DXIndexBuffer::getIndexCount() const {
		return _indexCount;
	}

	ID3D11Buffer* DXIndexBuffer::getIndexBuffer() const {
		return _indexBuffer;
	}
} // ciri