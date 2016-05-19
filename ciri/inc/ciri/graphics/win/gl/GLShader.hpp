#ifndef __ciri_graphics_GLShader__
#define __ciri_graphics_GLShader__

#include <memory>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <ciri/graphics/IShader.hpp>
#include <ciri/graphics/VertexDeclaration.hpp>

namespace ciri {

class IConstantBuffer;

class GLShader : public IShader {
public:
	GLShader();
	virtual ~GLShader();

	virtual void addInputElement( const VertexElement& element ) override;
	virtual ErrorCode loadFromFile( const char* vs, const char* gs, const char* ps ) override;
	virtual ErrorCode loadFromMemory( const char* vs, const char* gs, const char* ps ) override;
	virtual ErrorCode addConstants( const std::shared_ptr<IConstantBuffer>& buffer, const char* name, int shaderTypeFlags ) override;
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

}

#endif