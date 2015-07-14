#include <ciri/gfx/dx/DXVertexBuffer.hpp>
#include <ciri/gfx/dx/DXGraphicsDevice.hpp>

namespace ciri {
	DXVertexBuffer::DXVertexBuffer( DXGraphicsDevice* device )
		: IVertexBuffer(), _device(device), _vertexBuffer(nullptr), _vertexStride(0) {
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

	bool DXVertexBuffer::set( void* vertices, int vertexStride, int vertexCount, bool dynamic ) {
		// todo: add remapping (updating) support
		if( _vertexBuffer != nullptr ) {
			return false;
		}

		_vertexStride = vertexStride;

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
			return false;
		}

		return true;
	}

	int DXVertexBuffer::getStride() const {
		return _vertexStride;
	}

	ID3D11Buffer* DXVertexBuffer::getVertexBuffer() const {
		return _vertexBuffer;
	}
} // ciri