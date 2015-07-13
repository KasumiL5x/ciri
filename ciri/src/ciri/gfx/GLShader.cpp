#include <ciri/gfx/GLShader.hpp>
#include <ciri/gfx/GLGraphicsDevice.hpp>
#include <ciri/util/File.hpp>

namespace ciri {
	GLShader::GLShader( GLGraphicsDevice* device )
		: IShader(), _device(device), _vsFile(""), _gsFile(""), _psFile(""), _vertexShader(0), _geometryShader(0), _pixelShader(0), _program(0) {
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
	}

	bool GLShader::build() {
		if( _vsFile.empty() && _gsFile.empty() && _psFile.empty() ) {
			return false;
		}

		GLint status = GL_TRUE;

		// build the vertex shader
		if( !_vsFile.empty() ) {
			File file(_vsFile.c_str());
			if( !file.isOpen() ) {
				destroy();
				return false;
			}
			const std::string str = file.toString();
			const char* ptr = str.c_str();
			_vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(_vertexShader, 1, (const GLchar**)&ptr, 0);
			glCompileShader(_vertexShader);
			glGetShaderiv(_vertexShader, GL_COMPILE_STATUS, &status);
			if( status != GL_TRUE ) {
				destroy();
				return false;
			}
		}

		// build the geometry shader
		if( !_gsFile.empty() ) {
			File file(_gsFile.c_str());
			if( !file.isOpen() ) {
				destroy();
				return false;
			}
			const std::string str = file.toString();
			const char* ptr = str.c_str();
			_geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(_geometryShader, 1, (const GLchar**)&ptr, 0);
			glCompileShader(_geometryShader);
			glGetShaderiv(_geometryShader, GL_COMPILE_STATUS, &status);
			if( status != GL_TRUE ) {
				destroy();
				return false;
			}
		}

		// build the fragment shader
		if( !_psFile.empty() ) {
			File file(_psFile.c_str());
			if( !file.isOpen() ) {
				destroy();
				return false;
			}
			const std::string str = file.toString();
			const char* ptr = str.c_str();
			_pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(_pixelShader, 1, (const GLchar**)&ptr, 0);
			glCompileShader(_pixelShader);
			glGetShaderiv(_pixelShader, GL_COMPILE_STATUS, &status);
			if( status != GL_TRUE ) {
				destroy();
				return false;
			}
		}

		// todo: link

		// create the program and link the shaders
		_program = glCreateProgram();
		if( !_vsFile.empty() ) {
			glAttachShader(_program, _vertexShader);
			// todo: glBindAttribLocation(_program, index, "input") for all inputs
		}
		if( !_gsFile.empty() ) {
			glAttachShader(_program, _geometryShader);
		}
		if( !_psFile.empty() ) {
			glAttachShader(_program, _pixelShader);
			// todo: glBindFragDataLocation(_program, index, "output") for all outputs
		}
		glLinkProgram(_program);
		glGetProgramiv(_program, GL_LINK_STATUS, &status);
		if( status != GL_TRUE ) {
			destroy();
			return false;
		}

		return true;
	}

	bool GLShader::rebuild() {
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
		return "SUCCESS";
	}
} // ciri