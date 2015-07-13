#ifndef __ciri_glshader__
#define __ciri_glshader__

#include <string>
#include <gl/glew.h>
#include "../IShader.hpp"

namespace ciri {
	class GLGraphicsDevice;

	class GLShader : public IShader {
	public:
		GLShader( GLGraphicsDevice* device );
		virtual ~GLShader();

		virtual void addVertexShader( const char* filename );
		virtual void addGeometryShader( const char* filename );
		virtual void addPixelShader( const char* filename );
		virtual void addInputElement( const VertexElement& element );
		virtual bool build();
		virtual bool rebuild();
		virtual void destroy();
		virtual const char* getLastError() const;

	private:
		GLGraphicsDevice* _device;
		//
		std::string _vsFile;
		std::string _gsFile;
		std::string _psFile;
		//
		GLuint _vertexShader;
		GLuint _geometryShader;
		GLuint _pixelShader;
		GLuint _program;
	};
} // ciri

#endif /* __ciri_glshader__ */