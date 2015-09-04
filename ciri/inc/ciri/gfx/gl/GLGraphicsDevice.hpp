#ifndef __ciri_glgraphicsdevice__
#define __ciri_glgraphicsdevice__

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32s.lib")

#include <vector>
#include <string>
#include <Windows.h>
#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "../IGraphicsDevice.hpp"
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

		virtual bool create( Window* window );
		virtual void destroy();
		virtual void present();

		virtual IShader* createShader();
		virtual IVertexBuffer* createVertexBuffer();
		virtual IIndexBuffer* createIndexBuffer();
		virtual IConstantBuffer* createConstantBuffer();
		virtual ITexture2D* createTexture2D( int width, int height, TextureFormat::Format format, int flags, void* pixels=nullptr );
		virtual ITextureCube* createTextureCube( int width, int height, void* posx, void* negx, void* posy, void* negy, void* posz, void* negz ) override;
		virtual ISamplerState* createSamplerState( const SamplerDesc& desc );
		virtual IRenderTarget2D* createRenderTarget2D( int width, int height, TextureFormat::Format format );
		virtual IRasterizerState* createRasterizerState( const RasterizerDesc& desc );
		virtual IDepthStencilState* createDepthStencilState( const DepthStencilDesc& desc );
		virtual IBlendState* createBlendState( const BlendDesc& desc ) override;
		virtual void applyShader( IShader* shader );
		virtual void setVertexBuffer( IVertexBuffer* buffer );
		virtual void setIndexBuffer( IIndexBuffer* buffer );
		virtual void setTexture2D( int index, ITexture2D* texture, ShaderStage::Stage shaderStage );
		virtual void setTextureCube( int index, ITextureCube* texture, ShaderStage::Stage shaderStage ) override;
		virtual void setSamplerState( int index, ISamplerState* state, ShaderStage::Stage shaderStage );
		virtual void setBlendState( IBlendState* state ) override;
		virtual void drawArrays( PrimitiveTopology::Topology topology, int vertexCount, int startIndex );
		virtual void drawIndexed( PrimitiveTopology::Topology topology, int indexCount );
		virtual void setRenderTargets( IRenderTarget2D** renderTargets, int numRenderTargets );
		virtual void restoreDefaultRenderTargets();
		virtual err::ErrorCode resize();
		virtual void setClearColor( float r, float g, float b, float a );
		virtual void clear( int flags);
		virtual void setRasterizerState( IRasterizerState* state );
		virtual void setDepthStencilState( IDepthStencilState* state );
		virtual void setShaderExt( const char* ext );
		virtual const char* getShaderExt() const;
		virtual Window* getWindow() const;
		virtual const char* getGpuName() const;
		virtual const char* getApiInfo() const;
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
		Window* _window;
		//
		HDC _hdc;
		HGLRC _hglrc;
		int _defaultWidth;
		int _defaultHeight;
		//
		std::vector<GLShader*> _shaders;
		std::vector<GLVertexBuffer*> _vertexBuffers;
		std::vector<GLIndexBuffer*> _indexBuffers;
		//
		GLShader* _activeShader;
		GLVertexBuffer* _activeVertexBuffer;
		GLIndexBuffer* _activeIndexBuffer;
		//
		std::vector<GLConstantBuffer*> _constantBuffers;
		//
		std::vector<GLTexture2D*> _texture2Ds;
		//
		std::vector<GLSamplerState*> _samplers;
		//
		std::vector<GLRenderTarget2D*> _renderTarget2Ds;
		//
		GLuint _currentFbo;
		const static int MAX_MRTS{8};
		GLenum _drawBuffers[MAX_MRTS];
		//
		std::vector<GLRasterizerState*> _rasterizerStates;
		IRasterizerState* _activeRasterizerState;
		//
		std::vector<GLDepthStencilState*> _depthStencilStates;
		IDepthStencilState* _activeDepthStencilState;
		//
		std::string _shaderExt;
		//
		std::string _gpuName;
		std::string _apiInfo;
		//
		std::vector<GLBlendState*> _blendStates;
		//
		std::vector<GLTextureCube*> _textureCubes;
	};
} // ciri

#endif /* __ciri_glgraphicsdevice__ */
