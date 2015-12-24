#ifndef __ciri_gfx_dxtexture3d__
#define __ciri_gfx_dxtexture3d__

#include <memory>
#include <d3d11.h>
#include <ciri/gfx/ITexture3D.hpp>

namespace ciri {
	class DXGraphicsDevice;

	class DXTexture3D : public ITexture3D {
	public:
		DXTexture3D( int flags, const std::shared_ptr<DXGraphicsDevice>& device );
		virtual ~DXTexture3D();

		virtual void destroy() override;
		virtual ErrorCode setData( int width, int height, int depth, void* data, TextureFormat::Format format ) override;

		virtual int getWidth() const override;
		virtual int getHeight() const override;
		virtual int getDepth() const override;
		virtual TextureFormat::Format getFormat() const override;

		virtual ErrorCode writeToTGA( const char* file ) override;
		virtual ErrorCode writeToDDS( const char* file ) override;

		ID3D11Texture3D* getTexture() const;
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
		ID3D11Texture3D* _texture3D;
		ID3D11ShaderResourceView* _shaderResourceView;
		int _width;
		int _height;
		int _depth;
	};
} // ciri

#endif /* __ciri_gfx_dxtexture3d__ */
