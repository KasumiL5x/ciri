#include <ciri/gfx/gl/GLShader.hpp>
#include <ciri/gfx/gl/GLConstantBuffer.hpp>
#include <ciri/util/File.hpp>

namespace ciri {
	GLShader::GLShader()
		: IShader(), _vsFile(""), _gsFile(""), _psFile(""), _vertexShader(0), _geometryShader(0), _pixelShader(0), _program(0) {
	}

	GLShader::~GLShader() {
	}

	void GLShader::addVertexShader( const char* filename ) {
		_vsFile = filename;
	}

	void GLShader::addGeometryShader( const char* filename ) {
		_gsFile = filename;
	}

	void GLShader::addPixelShader( const char* filename ) {
		_psFile = filename;
	}

	void GLShader::addInputElement( const VertexElement& element ) {
		_vertexDeclaration.add(element);
	}

	err::ErrorCode GLShader::addConstants( IConstantBuffer* buffer, const char* name, int shaderTypeFlags ) {
		if( nullptr == buffer || nullptr == name ) {
			return err::CIRI_INVALID_ARGUMENT;
		}

		if( !isValid() ) {
			return err::CIRI_SHADER_INVALID;
		}

		GLConstantBuffer* glBuffer = reinterpret_cast<GLConstantBuffer*>(buffer);

		const GLuint blockIndex = glGetUniformBlockIndex(_program, name);
		if( GL_INVALID_INDEX == blockIndex ) {
			return err::CIRI_UNKNOWN_ERROR;
		}

		_constantBuffers.push_back(glBuffer);

		glBindBufferBase(GL_UNIFORM_BUFFER, _constantBuffers.size()-1, glBuffer->getUbo());
		glUniformBlockBinding(_program, blockIndex, _constantBuffers.size()-1);

		return err::CIRI_OK;
	}

	err::ErrorCode GLShader::build() {
		// must have at least VS and PS
		if( _vsFile.empty() && _psFile.empty() ) {
			return err::CIRI_SHADER_INCOMPLETE;
		}

		const int ERROR_LOG_SIZE = 1024;

		GLint status = GL_TRUE;

		// build the vertex shader
		if( !_vsFile.empty() ) {
			File file(_vsFile.c_str());
			if( !file.isOpen() ) {
				_lastError = err::getString(err::CIRI_FILE_NOT_FOUND) + std::string(" (") + _vsFile + std::string(")");;
				destroy();
				return err::CIRI_FILE_NOT_FOUND;
			}
			const std::string str = file.toString();
			const char* ptr = str.c_str();
			_vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(_vertexShader, 1, (const GLchar**)&ptr, 0);
			glCompileShader(_vertexShader);
			glGetShaderiv(_vertexShader, GL_COMPILE_STATUS, &status);
			if( status != GL_TRUE ) {
				GLchar log[ERROR_LOG_SIZE] = "";
				glGetShaderInfoLog(_vertexShader, ERROR_LOG_SIZE, 0, log);
				_lastError = err::getString(err::CIRI_SHADER_COMPILE_FAILED) + std::string(": ") + std::string(log);
				destroy();
				return err::CIRI_SHADER_COMPILE_FAILED;
			}
		}

		// build the geometry shader
		if( !_gsFile.empty() ) {
			File file(_gsFile.c_str());
			if( !file.isOpen() ) {
				_lastError = err::getString(err::CIRI_FILE_NOT_FOUND) + std::string(" (") + _gsFile + std::string(")");;
				destroy();
				return err::CIRI_FILE_NOT_FOUND;
			}
			const std::string str = file.toString();
			const char* ptr = str.c_str();
			_geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(_geometryShader, 1, (const GLchar**)&ptr, 0);
			glCompileShader(_geometryShader);
			glGetShaderiv(_geometryShader, GL_COMPILE_STATUS, &status);
			if( status != GL_TRUE ) {
				GLchar log[ERROR_LOG_SIZE] = "";
				glGetShaderInfoLog(_geometryShader, ERROR_LOG_SIZE, 0, log);
				_lastError = err::getString(err::CIRI_SHADER_COMPILE_FAILED) + std::string(": ") + std::string(log);
				destroy();
				return err::CIRI_SHADER_COMPILE_FAILED;
			}
		}

		// build the fragment shader
		if( !_psFile.empty() ) {
			File file(_psFile.c_str());
			if( !file.isOpen() ) {
				_lastError = err::getString(err::CIRI_FILE_NOT_FOUND) + std::string(" (") + _psFile + std::string(")");;
				destroy();
				return err::CIRI_FILE_NOT_FOUND;
			}
			const std::string str = file.toString();
			const char* ptr = str.c_str();
			_pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(_pixelShader, 1, (const GLchar**)&ptr, 0);
			glCompileShader(_pixelShader);
			glGetShaderiv(_pixelShader, GL_COMPILE_STATUS, &status);
			if( status != GL_TRUE ) {
				GLchar log[ERROR_LOG_SIZE] = "";
				glGetShaderInfoLog(_pixelShader, ERROR_LOG_SIZE, 0, log);
				_lastError = err::getString(err::CIRI_SHADER_COMPILE_FAILED) + std::string(": ") + std::string(log);
				destroy();
				return err::CIRI_SHADER_COMPILE_FAILED;
			}
		}

		// create the program and link the shaders
		_program = glCreateProgram();
		if( !_vsFile.empty() ) {
			glAttachShader(_program, _vertexShader);
		}
		if( !_gsFile.empty() ) {
			glAttachShader(_program, _geometryShader);
		}
		if( !_psFile.empty() ) {
			glAttachShader(_program, _pixelShader);
		}
		glLinkProgram(_program);
		glGetProgramiv(_program, GL_LINK_STATUS, &status);
		if( status != GL_TRUE ) {
			GLchar log[ERROR_LOG_SIZE] = "";
			glGetProgramInfoLog(_program, ERROR_LOG_SIZE, 0, log);
			_lastError = err::getString(err::CIRI_SHADER_LINK_FAILED) + std::string(": ") + std::string(log);
			destroy();
			return err::CIRI_SHADER_LINK_FAILED;
		}

		return err::CIRI_OK;
	}

	err::ErrorCode GLShader::rebuild() {
		destroy();
		return build();
	}

	void GLShader::destroy() {
		if( _vertexShader != 0 ) {
			glDeleteShader(_vertexShader);
			_vertexShader = 0;
		}

		if( _geometryShader != 0 ) {
			glDeleteShader(_geometryShader);
			_geometryShader = 0;
		}

		if( _pixelShader != 0 ) {
			glDeleteShader(_pixelShader);
			_pixelShader = 0;
		}

		if( _program != 0 ) {
			glDeleteProgram(_program);
			_program = 0;
		}
	}

	const char* GLShader::getLastError() const {
		return _lastError.c_str();
	}

	bool GLShader::isValid() const {
		return (_vertexShader != 0) && (_pixelShader != 0);
	}

	GLuint GLShader::getVertexShader() const {
		return _vertexShader;
	}

	GLuint GLShader::getGeometryShader() const {
		return _geometryShader;
	}

	GLuint GLShader::getPixelShader() const {
		return _pixelShader;
	}

	GLuint GLShader::getProgram() const {
		return _program;
	}

	const VertexDeclaration& GLShader::getVertexDeclaration() const {
		return _vertexDeclaration;
	}
} // ciri