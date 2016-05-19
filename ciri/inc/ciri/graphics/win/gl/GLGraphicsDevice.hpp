#ifndef __ciri_graphics_GLGraphicsDevice__
#define __ciri_graphics_GLGraphicsDevice__

#include <memory>
#include <string>
#include <Windows.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <ciri/graphics/IGraphicsDevice.hpp>
#include "GLShader.hpp"
#include "GLVertexBuffer.hpp"
#include "GLIndexBuffer.hpp"
#include "GLRasterizerState.hpp"
#include "GLDepthStencilState.hpp"

namespace ciri { namespace graphics {

class GLGraphicsDevice : public IGraphicsDevice {
public:
	GLGraphicsDevice();
	virtual ~GLGraphicsDevice();

	virtual bool create( const std::shared_ptr<core::IWindow>& window ) override;
	virtual void destroy() override;
	virtual void present() override;

	virtual void setViewport( const Viewport& vp ) override;
	virtual const Viewport& getViewport() const override;
	virtual std::shared_ptr<IShader> createShader() override;
	virtual std::shared_ptr<IVertexBuffer> createVertexBuffer() override;
	virtual std::shared_ptr<IIndexBuffer> createIndexBuffer() override;
	virtual std::shared_ptr<IConstantBuffer> createConstantBuffer() override;
	virtual std::shared_ptr<ITexture2D> createTexture2D( int width, int height, TextureFormat::Format format, int flags, void* pixels=nullptr ) override;
	virtual std::shared_ptr<ITexture3D> createTexture3D( int width, int height, int depth, TextureFormat::Format format, int flags, void* pixels=nullptr ) override;
	virtual std::shared_ptr<ITextureCube> createTextureCube( int width, int height, void* posx, void* negx, void* posy, void* negy, void* posz, void* negz ) override;
	virtual std::shared_ptr<ISamplerState> createSamplerState( const SamplerDesc& desc ) override;
	virtual std::shared_ptr<IRenderTarget2D> createRenderTarget2D( int width, int height, TextureFormat::Format format ) override;
	virtual std::shared_ptr<IRasterizerState> createRasterizerState( const RasterizerDesc& desc ) override;
	virtual std::shared_ptr<IDepthStencilState> createDepthStencilState( const DepthStencilDesc& desc ) override;
	virtual std::shared_ptr<IBlendState> createBlendState( const BlendDesc& desc ) override;
	virtual void applyShader( const std::shared_ptr<IShader>& shader ) override;
	virtual void setVertexBuffer( const std::shared_ptr<IVertexBuffer>& buffer ) override;
	virtual void setIndexBuffer( const std::shared_ptr<IIndexBuffer>& buffer ) override;
	virtual void setTexture2D( int index, const std::shared_ptr<ITexture2D>& texture, ShaderStage::Stage shaderStage ) override;
	virtual void setTexture3D( int index, const std::shared_ptr<ITexture3D>& texture, ShaderStage::Stage shaderStage ) override;
	virtual void setTextureCube( int index, const std::shared_ptr<ITextureCube>& texture, ShaderStage::Stage shaderStage ) override;
	virtual void setSamplerState( int index, const std::shared_ptr<ISamplerState>& state, ShaderStage::Stage shaderStage ) override;
	virtual void setBlendState( const std::shared_ptr<IBlendState>& state ) override;
	virtual void drawArrays( PrimitiveTopology topology, int vertexCount, int startIndex ) override;
	virtual void drawIndexed( PrimitiveTopology topology, int indexCount ) override;
	virtual void setRenderTargets( IRenderTarget2D** renderTargets, int numRenderTargets ) override;
	virtual void restoreDefaultRenderTargets() override;
	virtual core::ErrorCode resize() override;
	virtual core::ErrorCode resizeTexture2D( const std::shared_ptr<ITexture2D>& texture, int width, int height ) override;
	virtual core::ErrorCode resizeRenderTarget2D( const std::shared_ptr<IRenderTarget2D>& target, int width, int height ) override;
	virtual void setClearColor( float r, float g, float b, float a ) override;
	virtual void clear( int flags) override;
	virtual void setRasterizerState( const std::shared_ptr<IRasterizerState>& state ) override;
	virtual void setDepthStencilState( const std::shared_ptr<IDepthStencilState>& state ) override;
	virtual void setShaderExt( const char* ext ) override;
	virtual const char* getShaderExt() const override;
	virtual std::shared_ptr<core::IWindow> getWindow() const override;
	virtual const char* getGpuName() const override;
	virtual const char* getApiInfo() const override;
	virtual GraphicsApiType getApiType() const override;
	virtual core::ErrorCode restoreDefaultStates() override;
	virtual core::ErrorCode restoreDefaultBlendState() override;
	virtual core::ErrorCode restoreDefaultRasterizerState() override;
	virtual core::ErrorCode restoreDefaultDepthStencilState() override;
	virtual std::shared_ptr<IBlendState> getDefaultBlendAdditive() override;
	virtual std::shared_ptr<IBlendState> getDefaultBlendAlpha() override;
	virtual std::shared_ptr<IBlendState> getDefaultBlendNonPremul() override;
	virtual std::shared_ptr<IBlendState> getDefaultBlendOpaque() override;
	virtual std::shared_ptr<IRasterizerState> getDefaultRasterNone() override;
	virtual std::shared_ptr<IRasterizerState> getDefaultRasterClockwise() override;
	virtual std::shared_ptr<IRasterizerState> getDefaultRasterCounterClockwise() override;
	virtual std::shared_ptr<IDepthStencilState> getDefaultDepthStencilDefault() override;
	virtual std::shared_ptr<IDepthStencilState> getDefaultDepthStencilDepthRead() override;
	virtual std::shared_ptr<IDepthStencilState> getDefaultDepthStencilNone() override;

private:
	bool configureGl( HWND hwnd );
	bool configureGlew();
	// opengl debug messages
	static void APIENTRY debugContextCb( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam );
	static void APIENTRY debugContextAmdCb( GLuint id, GLenum category, GLenum severity, GLsizei length, const GLchar* message, void* userParam );
	// restore states
	//void restoreDefaultBlendState();

private:
	bool _isValid;
	std::shared_ptr<core::IWindow> _window;
	//
	HDC _hdc;
	HGLRC _hglrc;
	int _defaultWidth;
	int _defaultHeight;
	//
	Viewport _activeViewport;
	//
	std::weak_ptr<GLShader> _activeShader;
	std::weak_ptr<GLVertexBuffer> _activeVertexBuffer;
	std::weak_ptr<GLIndexBuffer> _activeIndexBuffer;
	//
	GLuint _currentFbo;
	const static int MAX_MRTS{8};
	GLenum _drawBuffers[MAX_MRTS];
	//
	std::weak_ptr<GLRasterizerState> _activeRasterizerState;
	//
	std::weak_ptr<GLDepthStencilState> _activeDepthStencilState;
	//
	std::string _shaderExt;
	//
	std::string _gpuName;
	std::string _apiInfo;
	//
	GLuint _dummyVao;
	//
	int _constantBufferCount;

	// default blend states
	std::shared_ptr<IBlendState> _defaultBlendAdditive;
	std::shared_ptr<IBlendState> _defaultBlendAlpha;
	std::shared_ptr<IBlendState> _defaultBlendNonPremul;
	std::shared_ptr<IBlendState> _defaultBlendOpaque;
	// default rasterizer states
	std::shared_ptr<IRasterizerState> _defaultRasterNone;
	std::shared_ptr<IRasterizerState> _defaultRasterClockwise;
	std::shared_ptr<IRasterizerState> _defaultRasterCounterClockwise;
	// default depth states
	std::shared_ptr<IDepthStencilState> _defaultDepthStencilDefault;
	std::shared_ptr<IDepthStencilState> _defaultDepthStencilDepthRead;
	std::shared_ptr<IDepthStencilState> _defaultDepthStencilNone;
};

}}

#endif