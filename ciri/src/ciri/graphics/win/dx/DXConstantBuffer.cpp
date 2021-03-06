#include <ciri/graphics/win/dx/DXConstantBuffer.hpp>
#include <ciri/graphics/win/dx/DXGraphicsDevice.hpp>

using namespace ciri;

DXConstantBuffer::DXConstantBuffer( const std::shared_ptr<DXGraphicsDevice>& device )
	: IConstantBuffer(), _device(device), _buffer(nullptr), _index(0) {
}

DXConstantBuffer::~DXConstantBuffer() {
	destroy();
}

ErrorCode DXConstantBuffer::setData( int dataSize, void* data ) {
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = dataSize;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA subdata;
	ZeroMemory(&subdata, sizeof(subdata));
	subdata.pSysMem = data;
	subdata.SysMemPitch = 0;
	subdata.SysMemSlicePitch = 0;

	if( nullptr == _buffer ) {
		HRESULT hr = _device->getDevice()->CreateBuffer(&desc, &subdata, &_buffer);
		if( FAILED(hr) ) {
			destroy();

			if( E_INVALIDARG == hr ) {
				return ErrorCode::CIRI_UNKNOWN_ERROR;
			}

			return ErrorCode::CIRI_UNKNOWN_ERROR;
		}
		return ErrorCode::CIRI_OK;
	}

	D3D11_MAPPED_SUBRESOURCE map;
	_device->getContext()->Map(_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	memcpy(map.pData, data, dataSize);
	_device->getContext()->Unmap(_buffer, 0);

	return ErrorCode::CIRI_OK;
}

void DXConstantBuffer::destroy() {
	if( _buffer != nullptr ) {
		_buffer->Release();
		_buffer = nullptr;
	}
}

ID3D11Buffer* DXConstantBuffer::getBuffer() const {
	return _buffer;
}

void DXConstantBuffer::setIndex( int val ) {
	_index = val;
}

int DXConstantBuffer::getIndex() const {
	return _index;
}