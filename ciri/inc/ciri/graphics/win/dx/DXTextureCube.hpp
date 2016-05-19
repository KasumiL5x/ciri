#ifndef __ciri_graphics_DXTextureCube__
#define __ciri_graphics_DXTextureCube__

#include <memory>
#include <d3d11.h>
#include <ciri/graphics/ITextureCube.hpp>

namespace ciri {

class DXGraphicsDevice;

class DXTextureCube : public ITextureCube {
public:
	DXTextureCube( const std::shared_ptr<DXGraphicsDevice>& device );
	virtual ~DXTextureCube();

	virtual ErrorCode set( int width, int height, void* posx, void* negx, void* posy, void* negy, void* posz, void* negz ) override;
	virtual void destroy() override;

	ID3D11Texture2D* getTexture() const;
	ID3D11ShaderResourceView* getShaderResourceView() const;

private:
	std::shared_ptr<DXGraphicsDevice> _device;
	ID3D11Texture2D* _cubeTexture;
	ID3D11ShaderResourceView* _shaderResourceView;
};

}

#endif