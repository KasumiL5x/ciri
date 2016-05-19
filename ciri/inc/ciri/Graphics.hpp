#ifndef __ciri_graphics_Graphics__
#define __ciri_graphics_Graphics__

#include <memory>
#include <ciri/graphics/BlendColorMask.hpp>
#include <ciri/graphics/BlendFunction.hpp>
#include <ciri/graphics/BlendMode.hpp>
#include <ciri/graphics/Camera.hpp>
#include <ciri/graphics/ClearFlags.hpp>
#include <ciri/graphics/CompareFunction.hpp>
#include <ciri/graphics/CullMode.hpp>
#include <ciri/graphics/FillMode.hpp>
#include <ciri/graphics/FPSCamera.hpp>
#include <ciri/graphics/GraphicsApiType.hpp>
#include <ciri/graphics/IBlendState.hpp>
#include <ciri/graphics/IConstantBuffer.hpp>
#include <ciri/graphics/IDepthStencilState.hpp>
#include <ciri/graphics/IGraphicsDevice.hpp>
#include <ciri/graphics/IIndexBuffer.hpp>
#include <ciri/graphics/IRasterizerState.hpp>
#include <ciri/graphics/IRenderTarget2D.hpp>
#include <ciri/graphics/ISamplerState.hpp>
#include <ciri/graphics/IShader.hpp>
#include <ciri/graphics/ITexture2D.hpp>
#include <ciri/graphics/ITexture3D.hpp>
#include <ciri/graphics/ITextureCube.hpp>
#include <ciri/graphics/IVertexBuffer.hpp>
#include <ciri/graphics/MayaCamera.hpp>
#include <ciri/graphics/ObjModel.hpp>
#include <ciri/graphics/Plane.hpp>
#include <ciri/graphics/PrimitiveTopology.hpp>
#include <ciri/graphics/SamplerFilter.hpp>
#include <ciri/graphics/SamplerWrap.hpp>
#include <ciri/graphics/ShaderStage.hpp>
#include <ciri/graphics/StencilOperation.hpp>
#include <ciri/graphics/TextureFlags.hpp>
#include <ciri/graphics/TextureFormat.hpp>
#include <ciri/graphics/VertexDeclaration.hpp>
#include <ciri/graphics/VertexElement.hpp>
#include <ciri/graphics/VertexFormat.hpp>
#include <ciri/graphics/VertexUsage.hpp>
#include <ciri/graphics/Viewport.hpp>

namespace ciri {

/**
	* Creates a new graphics device.
	* Note that this function is implemented in platform-specific code.
	* @returns Pointer to a new graphics device.
	*/
std::shared_ptr<IGraphicsDevice> createGraphicsDevice();

}

#endif