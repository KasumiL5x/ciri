#ifndef __ciri_igraphicsdevice__
#define __ciri_igraphicsdevice__

#include "IShader.hpp"
#include "IVertexBuffer.hpp"
#include "IIndexBuffer.hpp"
#include "PrimitiveTopology.hpp"
#include "ClearFlags.hpp"
#include "IConstantBuffer.hpp"
#include "ITexture2D.hpp"
#include "ShaderStage.hpp"
#include "ISamplerState.hpp"
#include "TextureFormat.hpp"
#include "TextureFlags.hpp"
#include "IRenderTarget2D.hpp"
#include "IRasterizerState.hpp"
#include "IDepthStencilState.hpp"

namespace ciri {
	class Window;

	class IGraphicsDevice {
	public:
		IGraphicsDevice() {
		}

		virtual ~IGraphicsDevice() {
		}

		/**
		 * Initializes the graphics device which will render to the provided Window.
		 * @param window Window to render to.
		 * @returns TODO
		 */
		virtual bool create( Window* window )=0;

		/**
		 * Uninitializes the graphics device and all of its managed resources that have not yet been freed.
		 */
		virtual void destroy()=0;

		/**
		 * Presents the backbuffer to the screen.
		 */
		virtual void present()=0;

		/**
		 * Creates a new shader.
		 * @returns A pointer to a new IShader, or nullptr upon error.
		 */
		virtual IShader* createShader()=0;

		/**
		 * Creates a new vertex buffer.
		 * @returns A pointer to a new IVertexBuffer, or nullptr upon error.
		 */
		virtual IVertexBuffer* createVertexBuffer()=0;

		/**
		 * Creates a new index buffer.
		 * @returns A pointer to a new IIndexBuffer, or nullptr upon error.
		 */
		virtual IIndexBuffer* createIndexBuffer()=0;

		/**
		 * Creates a new constant buffer.
		 * @returns A pointer to a new IConstantBuffer, or nullptr upon error.
		 */
		virtual IConstantBuffer* createConstantBuffer()=0;

		/**
		 * Creates a new 2d texture optionally initialized with data.
		 * @param width  Width of the texture in pixels.
		 * @param height Height of the texture in pixels.
		 * @param format Format of the texture.
		 * @param flags  Bitfield of optional TextureFlags.
		 * @param pixels Data to initialize the texture with.  If null, no specific data is set.
		 * @returns A pointer to a new ITexture2D of the given parameters, or nullptr upon error.
		 */
		virtual ITexture2D* createTexture2D( int width, int height, TextureFormat::Format format, int flags, void* pixels=nullptr )=0;

		/**
		 * Creates a new sampler state.
		 * @param desc Descriptor used to configure the sampler.
		 * @returns A pointer to a new ISamplerState, or nullptr upon error.
		 */
		virtual ISamplerState* createSamplerState( const SamplerDesc& desc )=0;

		/**
		 * Creates a new 2d render target.
		 * @param width  Width of the render target in pixels.
		 * @param height Height of the render target in pixels.
		 * @param format Format of the render target.
		 * @returns A pointer to a new IRenderTarget2D, or nullptr upon error.
		 */
		virtual IRenderTarget2D* createRenderTarget2D( int width, int height, TextureFormat::Format format )=0;

		/**
		 * Creates a new rasterizer state.
		 * @param desc Descriptor used to configure the state.
		 * @returns A pointer to a new IRasterizerState, or nullptr upon error.
		 */
		virtual IRasterizerState* createRasterizerState( const RasterizerDesc& desc )=0;

		/**
		 * Creates a new depth stencil state.
		 * @param desc Descriptor used to configure the state.
		 * @returns A pointer to a new IDepthStencilState, or nullptr upon error.
		 */
		virtual IDepthStencilState* createDepthStencilState( const DepthStencilDesc& desc )=0;

		/**
		 * Makes the given shader active.
		 * @param shader Shader to make active.
		 */
		virtual void applyShader( IShader* shader )=0;

		/**
		 * Makes the given vertex buffer active.
		 * @param buffer Vertex buffer to make active.
		 */
		virtual void setVertexBuffer( IVertexBuffer* buffer )=0;

		/**
		 * Makes the given index buffer active.
		 * @param buffer Index buffer to make active.
		 */
		virtual void setIndexBuffer( IIndexBuffer* buffer )=0;

		/**
		 * Binds a 2d texture to a slot at a given shader stage.
		 * @param index       Texture slot to bind to.
		 * @param texture     Texture to bind.
		 * @param shaderStage Stage at which to bind.
		 */
		virtual void setTexture2D( int index, ITexture2D* texture, ShaderStage::Stage shaderStage )=0;

		/**
		 * Binds a sampler state to a given slot at a given shader stage.
		 * @param index       Sampler slot to bind to.
		 * @param state       Sampler to bind.
		 * @param shaderStage Stage at which to bind.
		 */
		virtual void setSamplerState( int index, ISamplerState* state, ShaderStage::Stage shaderStage )=0;

		/**
		 * Draws primitives from the currently bound vertex buffer.
		 * @param topology    Topology to draw with.
		 * @param vertexCount Number of vertices to draw.
		 * @param startIndex  Offset into the bound vertex buffer of which to start.
		 */
		virtual void drawArrays( PrimitiveTopology::Topology topology, int vertexCount, int startIndex )=0;

		/**
		 * Draws indexed primitives from the currently bound vertex and index buffer.
		 * @param topology   Topology to draw with.
		 * @param indexCount Number of indices to draw.
		 */
		virtual void drawIndexed( PrimitiveTopology::Topology topology, int indexCount )=0;

		/**
		 * Sets active render targets.
		 * @param renderTargets    Array of pointers to IRenderTarget2D.
		 * @param numRenderTargets Number of render targets in the provided array.
		 */
		virtual void setRenderTargets( IRenderTarget2D** renderTargets, int numRenderTargets )=0;

		/**
		 * Restores the backbuffer and default depth stencil as the active render target.
		 */
		virtual void restoreDefaultRenderTargets()=0;

		/**
		 * Resizes the backbuffer and depth stencil to the bound window's size.
		 * @returns err::ErrorCode indicating success or failure.
		 */
		virtual err::ErrorCode resize()=0;

		/**
		 * Sets the color that will be applied to the active render targets when the clear function is called.
		 * @param r Red.
		 * @param g Green.
		 * @param b Blue.
		 * @param a Alpha.
		 */
		virtual void setClearColor( float r, float g, float b, float a )=0;

		/**
		 * Clears the currently bound render targets.
		 * @param flags Bitfield of ClearFlags defining what to clear.
		 */
		virtual void clear( int flags )=0;

		/**
		 * Sets the active rasterizer state.
		 * @param state Rasterizer state to make active.
		 */
		virtual void setRasterizerState( IRasterizerState* state )=0;

		/**
		 * Sets the active depth stencil state.
		 * @param state Depth stencil state to make active.
		 */
		virtual void setDepthStencilState( IDepthStencilState* state )=0;

		/**
		 * Sets the expected shader extension to be returned by getShaderExt.
		 * Note that this does not affect shader loading; it exists purely for convenience when targeting multiple graphics APIs.
		 * @param ext Extension to set.
		 */
		virtual void setShaderExt( const char* ext )=0;

		/**
		 * Gets the shader extension set by the user.
		 * Note that this does not affect shader loading; it exists purely for convenience when targeting multiple graphics APIs.
		 * @returns User-set string defining the shader file extension.
		 */
		virtual const char* getShaderExt() const=0;

		/**
		 * Gets the Window this device is rendering to.
		 * @returns Pointer to the attached window.
		 */
		virtual Window* getWindow() const=0;

		/**
		 * Gets the name of the GPU being used.
		 */
		virtual const char* getGpuName() const=0;

		/**
		 * Gets a string representing the underlying api version.
		 */
		virtual const char* getApiInfo() const=0;
	};
} // ciri

#endif /* __ciri_igraphicsdevice__ */
