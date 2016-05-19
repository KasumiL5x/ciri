#ifndef __ciri_graphics_DXTexture2D__
#define __ciri_graphics_DXTexture2D__

#include <memory>
#include <d3d11.h>
#include <ciri/graphics/ITexture2D.hpp>

namespace ciri { namespace graphics {

class DXGraphicsDevice;

class DXTexture2D : public ITexture2D {
public:
	DXTexture2D( int flags, const std::shared_ptr<DXGraphicsDevice>& device );
	virtual ~DXTexture2D();

	virtual void destroy() override;
	virtual core::ErrorCode setData( int xOffset, int yOffset, int width, int height, void* data, TextureFormat::Format format ) override;

	virtual int getWidth() const override;
	virtual int getHeight() const override;
	virtual TextureFormat::Format getFormat() const override;

	virtual core::ErrorCode writeToTGA( const char* file ) override;
	virtual core::ErrorCode writeToDDS( const char* file ) override;

	ID3D11Texture2D* getTexture() const;
	ID3D11ShaderResourceView* getShaderResourceView() const;

private:
	UINT getMipLevels() const;
	D3D11_USAGE getUsage() const;
	UINT getBindFlags() const;
	UINT getCpuFlags() const;
	UINT getMiscFlags() const;

private:
	std::shared_ptr<DXGraphicsDevice> _device;
	int _flags;
	TextureFormat::Format _format;
	ID3D11Texture2D* _texture2D;
	ID3D11ShaderResourceView* _shaderResourceView;
	int _width;
	int _height;
};

}}

#endif