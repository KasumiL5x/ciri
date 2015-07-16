#include <ciri/gfx/gl/GLGraphicsDevice.hpp>
#include <ciri/wnd/Window.hpp>

namespace ciri {
	GLGraphicsDevice::GLGraphicsDevice()
		: IGraphicsDevice(), _hdc(0), _hglrc(0), _activeShader(nullptr), _activeVertexBuffer(nullptr) {
	}

	GLGraphicsDevice::~GLGraphicsDevice() {
		destroy();
	}

	bool GLGraphicsDevice::create( Window* window ) {
		if( !configureGl(window->getHandle()) ) {
			destroy();
			return false;
		}

		if( !configureGlew() ) {
			destroy();
			return false;
		}

		return true;
	}

	void GLGraphicsDevice::destroy() {
		// destroy vertex buffers
		for( unsigned int i = 0; i < _vertexBuffers.size(); ++i ) {
			if( _vertexBuffers[i] != nullptr ) {
				_vertexBuffers[i]->destroy();
				delete _vertexBuffers[i];
				_vertexBuffers[i] = nullptr;
			}
		}
		_vertexBuffers.clear();

		// destroy shaders
		for( unsigned int i = 0; i < _shaders.size(); ++i ) {
			if( _shaders[i] != nullptr ) {
				_shaders[i]->destroy();
				delete _shaders[i];
				_shaders[i] = nullptr;
			}
		}
		_shaders.clear();

		// make rendering context not current
		wglMakeCurrent(0, 0);

		// delete the gl context
		if( _hglrc ) { wglDeleteContext(_hglrc); _hglrc = 0; }

		// delete device context
		if( _hdc ) { DeleteDC(_hdc); _hdc = 0; }
	}

	void GLGraphicsDevice::present() {
		SwapBuffers(_hdc);
	}

	IShader* GLGraphicsDevice::createShader() {
		GLShader* shader = new GLShader(this);
		_shaders.push_back(shader);
		return shader;
	}

	void GLGraphicsDevice::applyShader( IShader* shader ) {
		if( !shader->isValid() ) {
			_activeShader = nullptr;
			return;
		}

		GLShader* glShader = reinterpret_cast<GLShader*>(shader);
		glUseProgram(glShader->getProgram());
		_activeShader = glShader;
	}

	IVertexBuffer* GLGraphicsDevice::createVertexBuffer() {
		GLVertexBuffer* buffer = new GLVertexBuffer(this);
		_vertexBuffers.push_back(buffer);
		return buffer;
	}

	void GLGraphicsDevice::setVertexBuffer( IVertexBuffer* buffer ) {
		// cannot set parameters with no active shader
		if( nullptr == _activeShader ) {
			return;
		}

		GLVertexBuffer* glBuffer = reinterpret_cast<GLVertexBuffer*>(buffer);

		glBindBuffer(GL_ARRAY_BUFFER, glBuffer->getVbo());

		// apply vertex attribute pointers based on the shader's vertex declaration
		int offset = 0;
		const std::vector<VertexElement>& elements = _activeShader->getVertexDeclaration().getElements();
		for( unsigned int i = 0; i < elements.size(); ++i ) {
			const VertexElement& currElement = elements[i];

			glEnableVertexAttribArray(i);

			GLenum type;
			switch( currElement.getFormat() ) {
				case VertexFormat::Float:
				case VertexFormat::Float2:
				case VertexFormat::Float3:
				case VertexFormat::Float4: {
					type = GL_FLOAT;
				}
			}

			glVertexAttribPointer(i, currElement.getMultiplicity(), type, GL_FALSE, _activeShader->getVertexDeclaration().getStride(), (const void*)offset);
			offset += currElement.getSize(); // sizeof(datatype) * numberOfThem;
		}
	}

	void GLGraphicsDevice::drawArrays( PrimitiveTopology::Type topology, int vertexCount, int startIndex ) {
		// cannot draw with no active shader
		if( nullptr == _activeShader ) {
			return;
		}

		// cannot draw with no active vertex buffer
		if( nullptr == _activeVertexBuffer ) {
			return;
		}

		// vertex count must be greater than 0!
		if( vertexCount <= 0 ) {
			return;
		}

		// start index must be greater than zero and less than vertex count
		if( startIndex < 0 || startIndex >= vertexCount ) {
			return;
		}

		GLenum mode = 0;
		switch( topology ) {
			case PrimitiveTopology::PointList: {
				mode = GL_POINTS;
			}

			case PrimitiveTopology::LineList: {
				mode = GL_LINES; // wrong?
			}

			case PrimitiveTopology::LineStrip: {
				mode = GL_LINE_STRIP;
			}

			case PrimitiveTopology::TriangleList: {
				mode = GL_TRIANGLES; // wrong?
			}

			case PrimitiveTopology::TriangleStrip: {
				mode = GL_TRIANGLE_STRIP;
			}

			default: {
				break;
			}
		}

		glDrawArrays(mode, startIndex, vertexCount);
	}

	void GLGraphicsDevice::clear() {
		glClearColor(0.39f, 0.58f, 0.93f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	bool GLGraphicsDevice::configureGl( HWND hwnd ) {
		// get the window's device context
		_hdc = GetDC(hwnd);

		// create pixel format descriptor
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA, // The kind of framebuffer. RGBA or palette.
			32, // Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24, // Number of bits for the depthbuffer
			8, // Number of bits for the stencilbuffer
			0, // Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};
		// set pixel format of the device context
		const int pixelFormat = ChoosePixelFormat(_hdc, &pfd);
		if( 0 == pixelFormat ) {
			return false;
		}
		if( !SetPixelFormat(_hdc, pixelFormat, &pfd) ) {
			return false;
		}

		// create the opengl context
		_hglrc = wglCreateContext(_hdc);
		if( !_hglrc ) {
			return false;
		}

		// make the opengl context current
		if( !wglMakeCurrent(_hdc, _hglrc) ) {
			return false;
		}

		//TODO: setup gl debug message tracing

		return true;
	}

	bool GLGraphicsDevice::configureGlew() {
		return (glewInit() == GLEW_OK);
	}
} // ciri