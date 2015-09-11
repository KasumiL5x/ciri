#ifndef __ciri__
#define __ciri__

// core
#include "core/ErrorCodes.hpp"
#include "core/File.hpp"
#include "core/ITimer.hpp"
#include "core/Log.hpp"
#include "core/PNG.hpp"
#include "core/StrUtil.hpp"
#include "core/TGA.hpp"

// input
#include "input/IInput.hpp"
#include "input/Keyboard.hpp"
#include "input/Mouse.hpp"

// gfx
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

// wnd
#include "wnd/IWindow.hpp"
#include "wnd/WindowEvent.hpp"

#endif /* __ciri__ */