#include <ciri/gfx/win/gl/GLShader.hpp>
#include <ciri/gfx/win/gl/GLConstantBuffer.hpp>
#include <ciri/core/File.hpp>

namespace ciri {
	GLShader::GLShader()
		: IShader(), _vertexShader(0), _geometryShader(0), _pixelShader(0), _program(0) {
	}

	GLShader::~GLShader() {
	}

	void GLShader::addInputElement( const VertexElement& element ) {
		_vertexDeclaration.add(element);
	}

	ErrorCode GLShader::addConstants( const std::shared_ptr<IConstantBuffer>& buffer, const char* name, int shaderTypeFlags ) {
		if( nullptr == buffer || nullptr == name ) {
			return ErrorCode::CIRI_INVALID_ARGUMENT;
		}

		if( !isValid() ) {
			return ErrorCode::CIRI_SHADER_INVALID;
		}

		std::shared_ptr<GLConstantBuffer> glBuffer = std::static_pointer_cast<GLConstantBuffer>(buffer);

		const GLuint blockIndex = glGetUniformBlockIndex(_program, name);
		if( GL_INVALID_INDEX == blockIndex ) {
			return ErrorCode::CIRI_UNKNOWN_ERROR;
		}

		// note: I found out that uniform buffer objects work like texture units in that there's allocated space for them and then
		//       programs can bind to them as they see fit; the buffer indices themselves are per-context and not per-program.
		//       Here's some links I used for help just in case I need them for future reference:
		// http://www.opentk.com/node/2926
		// https://www.packtpub.com/books/content/opengl-40-using-uniform-blocks-and-uniform-buffer-objects
		// http://www.geeks3d.com/20140704/gpu-buffers-introduction-to-opengl-3-1-uniform-buffers-objects/
		// http://wlog.flatlib.jp/item/1634

		glBindBuffer(GL_UNIFORM_BUFFER, glBuffer->getUbo());
		glUniformBlockBinding(_program, blockIndex, glBuffer->getIndex());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		return ErrorCode::CIRI_OK;
	}

	ErrorCode GLShader::loadFromFile( const char* vs, const char* gs, const char* ps ) {
		clearErrors();

		// must have at least VS and PS
		if( nullptr == vs || nullptr == ps ) {
			addError(ErrorCode::CIRI_SHADER_INCOMPLETE, getErrorString(ErrorCode::CIRI_SHADER_INCOMPLETE));
			return ErrorCode::CIRI_SHADER_INCOMPLETE;
		}

		// load vs file
		File vsFile(vs, File::Flags::ReadOnly);
		if( !vsFile.isOpen() ) {
			addError(ErrorCode::CIRI_FILE_NOT_FOUND, getErrorString(ErrorCode::CIRI_FILE_NOT_FOUND) + std::string(" (") + vs + std::string(")"));
			return ErrorCode::CIRI_FILE_NOT_FOUND;
		}
		const std::string vsStr = vsFile.toString();

		// load gs file
		std::string gsStr = ""; // optional shader, so create empty string for it now
		if( gs != nullptr ) {
			File gsFile(gs, File::Flags::ReadOnly);
			if( !gsFile.isOpen() ) {
				addError(ErrorCode::CIRI_FILE_NOT_FOUND, getErrorString(ErrorCode::CIRI_FILE_NOT_FOUND) + std::string(" (") + gs + std::string(")"));
				return ErrorCode::CIRI_FILE_NOT_FOUND;
			}
			gsStr = gsFile.toString();
		}

		// load ps file
		File psFile(ps, File::Flags::ReadOnly);
		if( !psFile.isOpen() ) {
			addError(ErrorCode::CIRI_FILE_NOT_FOUND, getErrorString(ErrorCode::CIRI_FILE_NOT_FOUND) + std::string(" (") + ps + std::string(")"));
			return ErrorCode::CIRI_FILE_NOT_FOUND;
		}
		const std::string psStr = psFile.toString();

		return loadFromMemory(vsStr.c_str(), (nullptr==gs) ? nullptr : gsStr.c_str(), psStr.c_str());
	}

	ErrorCode GLShader::loadFromMemory( const char* vs, const char* gs, const char* ps ) {
		// todo: if valid, destroy and make new one

		clearErrors();

		// must have at least VS and PS
		if( nullptr == vs || nullptr == ps ) {
			addError(ErrorCode::CIRI_SHADER_INCOMPLETE, getErrorString(ErrorCode::CIRI_SHADER_INCOMPLETE));
			return ErrorCode::CIRI_SHADER_INCOMPLETE;
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
				addError(ErrorCode::CIRI_SHADER_COMPILE_FAILED, getErrorString(ErrorCode::CIRI_SHADER_COMPILE_FAILED) + std::string(": ") + std::string(log));
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
				addError(ErrorCode::CIRI_SHADER_COMPILE_FAILED, getErrorString(ErrorCode::CIRI_SHADER_COMPILE_FAILED) + std::string(": ") + std::string(log));
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
				addError(ErrorCode::CIRI_SHADER_COMPILE_FAILED, getErrorString(ErrorCode::CIRI_SHADER_COMPILE_FAILED) + std::string(": ") + std::string(log));
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
			addError(ErrorCode::CIRI_SHADER_LINK_FAILED, getErrorString(ErrorCode::CIRI_SHADER_LINK_FAILED) + std::string(": ") + std::string(log));
		}

		// process uniforms
		processUniforms();

		// if any errors have occurred, clean up and return the first error code
		if( !_errors.empty() ) {
			destroy();
			return _errors[0].code;
		}

		return ErrorCode::CIRI_OK;
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

	void GLShader::addError( ErrorCode code, const std::string& msg ) {
		_errors.push_back(ShaderError(code, msg));
	}

	void GLShader::clearErrors() {
		_errors.clear();
	}

	void GLShader::processUniforms() {
		// NOTE: It turns out that NVIDIA reorder the samplers (e.g. a samplerCube specified in the middle
		//       of other sampler2Ds will be reordered to the bottom) which completely breaks automatic
		//       binding using indices, as the shader's index is not the same as the reodered one after
		//       compilation.  Due to that, it is expected that all shaders now write their binding location
		//       by hand, akin to DirectX's register(...) command.  Fuck you, NVIDIA.  Fuck you.

		// note: assumes program is linked correctly

		//glUseProgram(_program);

		//// get the number of uniforms
		//GLint numUniforms = 0;
		//glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &numUniforms);

		//// get max length of uniform
		//GLint maxUniformLength = 0;
		//glGetProgramiv(_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLength);

		//// will store the name of uniforms
		//GLchar* name = new GLchar[maxUniformLength];

		//// total number of samplers
		//int samplerCount = 0;

		//for( int i = 0; i < numUniforms; ++i ) {
		//	GLsizei actualLength; GLint size; GLenum type;
		//	glGetActiveUniform(_program, i, maxUniformLength, &actualLength, &size, &type, name);

		//	// process all samplers
		//	if( type >= GL_SAMPLER_1D && type <= GL_UNSIGNED_INT_SAMPLER_2D_RECT ) {
		//		glUniform1i(glGetUniformLocation(_program, name), samplerCount);
		//		samplerCount += 1;
		//		continue;
		//	}
		//}

		//delete[] name;
		//name = nullptr;

		//glUseProgram(0);
	}
} // ciri