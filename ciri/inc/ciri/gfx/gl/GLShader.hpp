#ifndef __ciri_glshader__
#define __ciri_glshader__

#include <vector>
#include <string>
#include <gl/glew.h>
#include "../IShader.hpp"
#include "../VertexDeclaration.hpp"

namespace ciri {
	class IConstantBuffer;

	class GLShader : public IShader {
	public:
		GLShader();
		virtual ~GLShader();

		virtual void addInputElement( const VertexElement& element );
		virtual err::ErrorCode loadFromFile( const char* vs, const char* gs, const char* ps );
		virtual err::ErrorCode loadFromMemory( const char* vs, const char* gs, const char* ps );
		virtual err::ErrorCode addConstants( IConstantBuffer* buffer, const char* name, int shaderTypeFlags );
		virtual void destroy();
		virtual const std::vector<ShaderError>& getErrors() const;
		virtual bool isValid() const;

		GLuint getVertexShader() const;
		GLuint getGeometryShader() const;
		GLuint getPixelShader() const;
		GLuint getProgram() const;
		const VertexDeclaration& getVertexDeclaration() const;

	private:
		void addError( err::ErrorCode code, const std::string& msg );
		void clearErrors();
		void processUniforms();

	private:
		GLuint _vertexShader;
		GLuint _geometryShader;
		GLuint _pixelShader;
		GLuint _program;
		//
		std::vector<ShaderError> _errors;
		//
		VertexDeclaration _vertexDeclaration;
	};
} // ciri

#endif /* __ciri_glshader__ */