#include <ciri/gfx/gl/GLShader.hpp>
#include <ciri/gfx/gl/GLConstantBuffer.hpp>
#include <ciri/util/File.hpp>

namespace ciri {
	GLShader::GLShader()
		: IShader(), _vertexShader(0), _geometryShader(0), _pixelShader(0), _program(0) {
	}

	GLShader::~GLShader() {
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

		// note: I found out that uniform buffer objects work like texture units in that there's allocated space for them and then
		//       programs can bind to them as they see fit; the buffer indices themselves are per-context and not per-program.
		//       Here's some links I used for help just in case I need them for future reference:
		// http://www.opentk.com/node/2926
		// https://www.packtpub.com/books/content/opengl-40-using-uniform-blocks-and-uniform-buffer-objects
		// http://www.geeks3d.com/20140704/gpu-buffers-introduction-to-opengl-3-1-uniform-buffers-objects/
		// http://wlog.flatlib.jp/item/1634

		_constantBuffers.push_back(glBuffer);

		glBindBuffer(GL_UNIFORM_BUFFER, glBuffer->getUbo());
		glUniformBlockBinding(_program, blockIndex, glBuffer->getIndex());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		return err::CIRI_OK;
	}

	err::ErrorCode GLShader::loadFromFile( const char* vs, const char* gs, const char* ps ) {
		clearErrors();

		// must have at least VS and PS
		if( nullptr == vs || nullptr == ps ) {
			addError(err::CIRI_SHADER_INCOMPLETE, err::getString(err::CIRI_SHADER_INCOMPLETE));
			return err::CIRI_SHADER_INCOMPLETE;
		}

		// load vs file
		File vsFile(vs);
		if( !vsFile.isOpen() ) {
			addError(err::CIRI_FILE_NOT_FOUND, err::getString(err::CIRI_FILE_NOT_FOUND) + std::string(" (") + vs + std::string(")"));
			return err::CIRI_FILE_NOT_FOUND;
		}
		const std::string vsStr = vsFile.toString();

		// load gs file
		std::string gsStr = ""; // optional shader, so create empty string for it now
		if( gs != nullptr ) {
			File gsFile(gs);
			if( !gsFile.isOpen() ) {
				addError(err::CIRI_FILE_NOT_FOUND, err::getString(err::CIRI_FILE_NOT_FOUND) + std::string(" (") + gs + std::string(")"));
				return err::CIRI_FILE_NOT_FOUND;
			}
			gsStr = gsFile.toString();
		}

		// load ps file
		File psFile(ps);
		if( !psFile.isOpen() ) {
			addError(err::CIRI_FILE_NOT_FOUND, err::getString(err::CIRI_FILE_NOT_FOUND) + std::string(" (") + ps + std::string(")"));
			return err::CIRI_FILE_NOT_FOUND;
		}
		const std::string psStr = psFile.toString();

		return loadFromMemory(vsStr.c_str(), (nullptr==gs) ? nullptr : gsStr.c_str(), psStr.c_str());
	}

	err::ErrorCode GLShader::loadFromMemory( const char* vs, const char* gs, const char* ps ) {
		// todo: if valid, destroy and make new one

		clearErrors();

		// must have at least VS and PS
		if( nullptr == vs || nullptr == ps ) {
			addError(err::CIRI_SHADER_INCOMPLETE, err::getString(err::CIRI_SHADER_INCOMPLETE));
			return err::CIRI_SHADER_INCOMPLETE;
		}

		const int ERROR_LOG_SIZE = 1024;
		GLint status = GL_TRUE;

		// build the vertex shader
		{
			_vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(_vertexShader, 1, (const GLchar**)&vs, 0);
			glCompileShader(_vertexShader);
			glGetShaderiv(_vertexShader, GL_COMPILE_STATUS, &status);
			if( status != GL_TRUE ) {
				GLchar log[ERROR_LOG_SIZE] = "";
				glGetShaderInfoLog(_vertexShader, ERROR_LOG_SIZE, 0, log);
				addError(err::CIRI_SHADER_COMPILE_FAILED, err::getString(err::CIRI_SHADER_COMPILE_FAILED) + std::string(": ") + std::string(log));
			}
		}

		// build the geometry shader
		if( gs != nullptr ) {
			_geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(_geometryShader, 1, (const GLchar**)&gs, 0);
			glCompileShader(_geometryShader);
			glGetShaderiv(_geometryShader, GL_COMPILE_STATUS, &status);
			if( status != GL_TRUE ) {
				GLchar log[ERROR_LOG_SIZE] = "";
				glGetShaderInfoLog(_geometryShader, ERROR_LOG_SIZE, 0, log);
				addError(err::CIRI_SHADER_COMPILE_FAILED, err::getString(err::CIRI_SHADER_COMPILE_FAILED) + std::string(": ") + std::string(log));
			}
		}

		// build the fragment shader
		{
			_pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(_pixelShader, 1, (const GLchar**)&ps, 0);
			glCompileShader(_pixelShader);
			glGetShaderiv(_pixelShader, GL_COMPILE_STATUS, &status);
			if( status != GL_TRUE ) {
				GLchar log[ERROR_LOG_SIZE] = "";
				glGetShaderInfoLog(_pixelShader, ERROR_LOG_SIZE, 0, log);
				addError(err::CIRI_SHADER_COMPILE_FAILED, err::getString(err::CIRI_SHADER_COMPILE_FAILED) + std::string(": ") + std::string(log));
			}
		}

		// create the program and link the shaders
		_program = glCreateProgram();
		glAttachShader(_program, _vertexShader);
		if( gs != nullptr ) {
			glAttachShader(_program, _geometryShader);
		}
		glAttachShader(_program, _pixelShader);
		glLinkProgram(_program);
		glGetProgramiv(_program, GL_LINK_STATUS, &status);
		if( status != GL_TRUE ) {
			GLchar log[ERROR_LOG_SIZE] = "";
			glGetProgramInfoLog(_program, ERROR_LOG_SIZE, 0, log);
			addError(err::CIRI_SHADER_LINK_FAILED, err::getString(err::CIRI_SHADER_LINK_FAILED) + std::string(": ") + std::string(log));
		}

		// if any errors have occurred, clean up and return the first error code
		if( !_errors.empty() ) {
			destroy();
			return _errors[0].code;
		}

		return err::CIRI_OK;
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

	const std::vector<IShader::ShaderError>& GLShader::getErrors() const {
		return _errors;
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

	void GLShader::addError( err::ErrorCode code, const std::string& msg ) {
		_errors.push_back(ShaderError(code, msg));
	}

	void GLShader::clearErrors() {
		_errors.clear();
	}
} // ciri