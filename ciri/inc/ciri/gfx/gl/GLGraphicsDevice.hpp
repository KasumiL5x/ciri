#ifndef __ciri_glgraphicsdevice__
#define __ciri_glgraphicsdevice__

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32s.lib")

#include <vector>
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
		virtual ITexture2D* createTexture2D();
		virtual ISamplerState* createSamplerState( const SamplerDesc& desc );
		virtual IRenderTarget2D* createRenderTarget2D( int width, int height, TextureFormat::Type format );
		virtual IRasterizerState* createRasterizerState( const RasterizerDesc& desc );
		virtual IDepthStencilState* createDepthStencilState( const DepthStencilDesc& desc );

		virtual void applyShader( IShader* shader );

		virtual void setVertexBuffer( IVertexBuffer* buffer );
		virtual void setIndexBuffer( IIndexBuffer* buffer );

		virtual void setTexture2D( int index, ITexture2D* texture, ShaderStage::Stage shaderStage );
		virtual void setSamplerState( int index, ISamplerState* state, ShaderStage::Stage shaderStage );

		virtual void drawArrays( PrimitiveTopology::Type topology, int vertexCount, int startIndex );
		virtual void drawIndexed( PrimitiveTopology::Type topology, int indexCount );

		virtual void setRenderTargets( IRenderTarget2D** renderTargets, int numRenderTargets );
		virtual void restoreDefaultRenderTargets();
		virtual void setClearColor( float r, float g, float b, float a );
		virtual void clear( int flags);

		virtual void setRasterizerState( IRasterizerState* state );
		virtual void setDepthStencilState( IDepthStencilState* state );

	private:
		bool configureGl( HWND hwnd );
		bool configureGlew();
		GLenum convertTopology( PrimitiveTopology::Type topology ) const;
		// opengl debug messages
		static void APIENTRY debugContextCb( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam );
		static void APIENTRY debugContextAmdCb( GLuint id, GLenum category, GLenum severity, GLsizei length, const GLchar* message, void* userParam );

	private:
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
	};
} // ciri

#endif /* __ciri_glgraphicsdevice__ */
