#ifndef __ciri_graphics__
#define __ciri_graphics__

#include <memory>
#include "gfx/BlendColorMask.hpp"
#include "gfx/BlendFunction.hpp"
#include "gfx/BlendMode.hpp"
#include "gfx/Camera.hpp"
#include "gfx/ClearFlags.hpp"
#include "gfx/CompareFunction.hpp"
#include "gfx/CullMode.hpp"
#include "gfx/FillMode.hpp"
#include "gfx/FPSCamera.hpp"
#include "gfx/GraphicsApiType.hpp"
#include "gfx/IBlendState.hpp"
#include "gfx/IConstantBuffer.hpp"
#include "gfx/IDepthStencilState.hpp"
#include "gfx/IGraphicsDevice.hpp"
#include "gfx/IIndexBuffer.hpp"
#include "gfx/IRasterizerState.hpp"
#include "gfx/IRenderTarget2D.hpp"
#include "gfx/ISamplerState.hpp"
#include "gfx/IShader.hpp"
#include "gfx/ITexture2D.hpp"
#include "gfx/ITextureCube.hpp"
#include "gfx/IVertexBuffer.hpp"
#include "gfx/MayaCamera.hpp"
#include "gfx/ObjModel.hpp"
#include "gfx/Plane.hpp"
#include "gfx/PrimitiveTopology.hpp"
#include "gfx/SamplerFilter.hpp"
#include "gfx/SamplerWrap.hpp"
#include "gfx/ShaderStage.hpp"
#include "gfx/StencilOperation.hpp"
#include "gfx/TextureFlags.hpp"
#include "gfx/TextureFormat.hpp"
#include "gfx/VertexDeclaration.hpp"
#include "gfx/VertexElement.hpp"
#include "gfx/VertexFormat.hpp"
#include "gfx/VertexUsage.hpp"
#include "gfx/Viewport.hpp"

namespace ciri {
	/**
	 * Creates a new graphics device.
	 * Note that this function is defined in the linked graphics library, not in ciri itself.
	 * @returns Pointer to a new graphics device.
	 */
	std::shared_ptr<IGraphicsDevice> createGraphicsDevice();
} // ciri

#endif /* __ciri_graphics__ */