#ifndef __ciri_glgraphicsdevice__
#define __ciri_glgraphicsdevice__

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32s.lib")

#include <vector>
#include <Windows.h>
#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "../IGraphicsDevice.hpp"
#include "GLShader.hpp"
#include "GLVertexBuffer.hpp"
#include "GLConstantBuffer.hpp"

namespace ciri {
	class GLGraphicsDevice : public IGraphicsDevice {
	public:
		GLGraphicsDevice();
		virtual ~GLGraphicsDevice();

		virtual bool create( Window* window );
		virtual void destroy();
		virtual void present();
		virtual IShader* createShader();
		virtual void applyShader( IShader* shader );
		virtual IVertexBuffer* createVertexBuffer();
		virtual void setVertexBuffer( IVertexBuffer* buffer );
		virtual void drawArrays( PrimitiveTopology::Type topology, int vertexCount, int startIndex );
		virtual void clear();
		virtual IConstantBuffer* createConstantBuffer();

	private:
		bool configureGl( HWND hwnd );
		bool configureGlew();

	private:
		HDC _hdc;
		HGLRC _hglrc;
		//
		std::vector<GLShader*> _shaders;
		std::vector<GLVertexBuffer*> _vertexBuffers;
		//
		GLShader* _activeShader;
		GLVertexBuffer* _activeVertexBuffer;
		//
		std::vector<GLConstantBuffer*> _constantBuffers;
	};
} // ciri

#endif /* __ciri_glgraphicsdevice__ */