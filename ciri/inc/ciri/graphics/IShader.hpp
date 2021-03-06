#ifndef __ciri_graphics_IShader__
#define __ciri_graphics_IShader__

#include <memory>
#include <ciri/core/ErrorCodes.hpp>
#include "VertexElement.hpp"

namespace ciri {

class IConstantBuffer;

class IShader {
public:
	struct ShaderError {
		ErrorCode code;
		std::string msg;

		ShaderError() {
		}
		ShaderError( ErrorCode theCode, const std::string& theMsg )
			: code(theCode), msg(theMsg) {
		}
	};

protected:
	IShader() {
	}

public:
	virtual ~IShader() {
	}

	/**
		* Adds an input element defining the input vertex structure to the shader.
		* @param element Element to add.
		*/
	virtual void addInputElement( const VertexElement& element )=0;

	/**
		* Builds the shader from existing files.  Both the vertex shader and pixel shader must be valid, but the geometry shader is optional.
		* @param vs Vertex shader file.  This must not be null.
		* @param gs Geometry shader file.  This can optionally be null.
		* @param ps Pixel shader file.  This must not be null.
		* @returns ErrorCode indicating success or failure.  If multiple errors exist, the first to happen will be returned.
		*/
	virtual ErrorCode loadFromFile( const char* vs, const char* gs, const char* ps )=0;

	/**
		* Builds the shader from memory.  Both the vertex shader and pixel shader must be valid, but the geometry shader is optional.
		* @param vs Vertex shader string.  This must not be null.
		* @param gs Geometry shader string.  This can optionally be null.
		* @param ps Pixel shader string.  This must not be null.
		* @returns ErrorCode indicating success or failure.  If multiple errors exist, the first to happen will be returned.
		*/
	virtual ErrorCode loadFromMemory( const char* vs, const char* gs, const char* ps )=0;

	/**
		* Attaches a constant buffer to the shader at a specific stage.
		* Note that the constant buffer itself must exist within the shaders with the same name as is specified, else will error.
		* @param buffer          Pointer to the IConstantBuffer to attach.
		* @param name            Name of the constant buffer in the shader.  This must already exist in the shader.
		* @param shaderTypeFlags Bitflag of ShaderStage to define which stage(s) the constant buffer is bound to.
		* @returns ErrorCode indicating success or failure.
		*/
	virtual ErrorCode addConstants( const std::shared_ptr<IConstantBuffer>& buffer, const char* name, int shaderTypeFlags )=0;

	/**
		* Uninitializes the shader.
		*/
	virtual void destroy()=0;

	/**
		* Gets a vector of errors that have occurred.  Previously accumulated errors are discarded for new loads.
		* @returns Vector of ShaderErrors.
		*/
	virtual const std::vector<ShaderError>& getErrors() const=0;

	/**
		* Checks if the shader is valid.
		* A shader is considered valid if it has both a compiled vertex and pixel shader.
		* @return True if valid.
		*/
	virtual bool isValid() const=0;
};

}

#endif