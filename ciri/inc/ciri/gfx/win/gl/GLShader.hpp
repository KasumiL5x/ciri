#ifndef __ciri_gfx_glshader__
#define __ciri_gfx_glshader__

#include <vector>
#include <string>
#include <gl/glew.h>
#include <ciri/gfx/IShader.hpp>
#include <ciri/gfx/VertexDeclaration.hpp>

namespace ciri {
	class IConstantBuffer;

	class GLShader : public IShader {
	public:
		GLShader();
		virtual ~GLShader();

		virtual void addInputElement( const VertexElement& element ) override;
		virtual ErrorCode loadFromFile( const char* vs, const char* gs, const char* ps ) override;
		virtual ErrorCode loadFromMemory( const char* vs, const char* gs, const char* ps ) override;
		virtual ErrorCode addConstants( IConstantBuffer* buffer, const char* name, int shaderTypeFlags ) override;
		virtual void destroy() override;
		virtual const std::vector<ShaderError>& getErrors() const override;
		virtual bool isValid() const override;

		GLuint getVertexShader() const;
		GLuint getGeometryShader() const;
		GLuint getPixelShader() const;
		GLuint getProgram() const;
		const VertexDeclaration& getVertexDeclaration() const;

	private:
		void addError( ErrorCode code, const std::string& msg );
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

#endif /* __ciri_gfx_glshader__ */
