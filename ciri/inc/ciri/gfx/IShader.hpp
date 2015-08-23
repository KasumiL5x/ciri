#ifndef __ciri_ishader__
#define __ciri_ishader__

#include "VertexElement.hpp"
#include "../ErrorCodes.hpp"

namespace ciri {
	class IConstantBuffer;

	class IShader {
	public:
		IShader() {
		}
		virtual ~IShader() {
		}

		/**
		 * Sets the file to load as a vertex shader when building.
		 * @param filename Filename of the standalone vertex shader.
		 */
		virtual void addVertexShader( const char* filename )=0;

		/**
		 * Sets the file to load as a geometry shader when building.
		 * @param filename Filename of the standalone geometry shader.
		 */
		virtual void addGeometryShader( const char* filename )=0;

		/**
		 * Sets the file to load as a pixel shader when building.
		 * @param filename Filename of the standalone pixel shader.
		 */
		virtual void addPixelShader( const char* filename )=0;

		/**
		 * Adds an input element defining the input vertex structure to the shader.
		 * @param element Element to add.
		 */
		virtual void addInputElement( const VertexElement& element )=0;

		/**
		 * Attaches a constant buffer to the shader at a specific stage.
		 * Note that the constant buffer itself must exist within the shaders with the same name as is specified, else will error.
		 * @param buffer          Pointer to the IConstantBuffer to attach.
		 * @param name            Name of the constant buffer in the shader.  This must already exist in the shader.
		 * @param shaderTypeFlags Bitflag of ShaderStage to define which stage(s) the constant buffer is bound to.
		 * @returns err::ErrorCode indicating success or failure.
		 */
		virtual err::ErrorCode addConstants( IConstantBuffer* buffer, const char* name, int shaderTypeFlags )=0;

		/**
		 * Builds the shader using already set parameters.
		 * Note that the last occurred error can be retrieved with getLastError().
		 * @returns err::ErrorCode indicating success or failure.
		 */
		virtual err::ErrorCode build()=0;

		/**
		 * Rebuild an already build shader (for instance, if a shader file was modified).
		 * @returns err::ErrorCode indicating success or failure.
		 */
		virtual err::ErrorCode rebuild()=0;

		/**
		 * Uninitializes the shader.
		 */
		virtual void destroy()=0;

		/**
		 * Gets the last error that occurred.
		 * @return Last error string.
		 */
		virtual const char* getLastError() const=0;

		/**
		 * Checks if the shader is valid.
		 * A shader is considered valid if it has both a compiled vertex and pixel shader.
		 * @return True if valid.
		 */
		virtual bool isValid() const=0;
	};
} // ciri

#endif /* __ciri_ishader__ */
