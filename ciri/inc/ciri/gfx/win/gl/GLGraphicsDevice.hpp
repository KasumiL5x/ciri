#ifndef __ciri_gfx_glgraphicsdevice__
#define __ciri_gfx_glgraphicsdevice__

#include <vector>
#include <string>
#include <Windows.h>
#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <ciri/gfx/IGraphicsDevice.hpp>
#include "GLShader.hpp"
#include "GLVertexBuffer.hpp"
#include "GLIndexBuffer.hpp"
#include "GLConstantBuffer.hpp"
#include "GLTexture2D.hpp"
#include "GLSamplerState.hpp"
#include "GLRenderTarget2D.hpp"
#include "GLRasterizerState.hpp"
#include "GLDepthStencilState.hpp"
#include "GLBlendState.hpp"
#include "GLTextureCube.hpp"

namespace ciri {
	class GLGraphicsDevice : public IGraphicsDevice {
	public:
		GLGraphicsDevice();
		virtual ~GLGraphicsDevice();

		virtual bool create( const std::shared_ptr<IWindow>& window ) override;
		virtual void destroy() override;
		virtual void present() override;

		virtual std::shared_ptr<IShader> createShader() override;
		virtual std::shared_ptr<IVertexBuffer> createVertexBuffer() override;
		virtual std::shared_ptr<IIndexBuffer> createIndexBuffer() override;
		virtual std::shared_ptr<IConstantBuffer> createConstantBuffer() override;
		virtual std::shared_ptr<ITexture2D> createTexture2D( int width, int height, TextureFormat::Format format, int flags, void* pixels=nullptr ) override;
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
		virtual void setTextureCube( int index, const std::shared_ptr<ITextureCube>& texture, ShaderStage::Stage shaderStage ) override;
		virtual void setSamplerState( int index, const std::shared_ptr<ISamplerState>& state, ShaderStage::Stage shaderStage ) override;
		virtual void setBlendState( const std::shared_ptr<IBlendState>& state ) override;
		virtual void drawArrays( PrimitiveTopology::Topology topology, int vertexCount, int startIndex ) override;
		virtual void drawIndexed( PrimitiveTopology::Topology topology, int indexCount ) override;
		virtual void setRenderTargets( IRenderTarget2D** renderTargets, int numRenderTargets ) override;
		virtual void restoreDefaultRenderTargets() override;
		virtual ErrorCode resize() override;
		virtual void setClearColor( float r, float g, float b, float a ) override;
		virtual void clear( int flags) override;
		virtual void setRasterizerState( const std::shared_ptr<IRasterizerState>& state ) override;
		virtual void setDepthStencilState( const std::shared_ptr<IDepthStencilState>& state ) override;
		virtual void setShaderExt( const char* ext ) override;
		virtual const char* getShaderExt() const override;
		virtual std::shared_ptr<IWindow> getWindow() const override;
		virtual const char* getGpuName() const override;
		virtual const char* getApiInfo() const override;
		virtual GraphicsApiType::Type getApiType() const override;

	private:
		bool configureGl( HWND hwnd );
		bool configureGlew();
		// opengl debug messages
		static void APIENTRY debugContextCb( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam );
		static void APIENTRY debugContextAmdCb( GLuint id, GLenum category, GLenum severity, GLsizei length, const GLchar* message, void* userParam );
		// restore states
		void restoreDefaultBlendState();

	private:
		bool _isValid;
		std::shared_ptr<IWindow> _window;
		//
		HDC _hdc;
		HGLRC _hglrc;
		int _defaultWidth;
		int _defaultHeight;
		//
		std::vector<std::shared_ptr<GLShader>> _shaders;
		std::vector<std::shared_ptr<GLVertexBuffer>> _vertexBuffers;
		std::vector<std::shared_ptr<GLIndexBuffer>> _indexBuffers;
		//
		GLShader* _activeShader;
		GLVertexBuffer* _activeVertexBuffer;
		GLIndexBuffer* _activeIndexBuffer;
		//
		std::vector<std::shared_ptr<GLConstantBuffer>> _constantBuffers;
		//
		std::vector<std::shared_ptr<GLTexture2D>> _texture2Ds;
		//
		std::vector<std::shared_ptr<GLSamplerState>> _samplers;
		//
		std::vector<std::shared_ptr<GLRenderTarget2D>> _renderTarget2Ds;
		//
		GLuint _currentFbo;
		const static int MAX_MRTS{8};
		GLenum _drawBuffers[MAX_MRTS];
		//
		std::vector<std::shared_ptr<GLRasterizerState>> _rasterizerStates;
		GLRasterizerState* _activeRasterizerState;
		//
		std::vector<std::shared_ptr<GLDepthStencilState>> _depthStencilStates;
		GLDepthStencilState* _activeDepthStencilState;
		//
		std::string _shaderExt;
		//
		std::string _gpuName;
		std::string _apiInfo;
		//
		std::vector<std::shared_ptr<GLBlendState>> _blendStates;
		//
		std::vector<std::shared_ptr<GLTextureCube>> _textureCubes;
	};
} // ciri

#endif /* __ciri_gfx_glgraphicsdevice__ */
