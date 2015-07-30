#include <ciri/gfx/gl/GLGraphicsDevice.hpp>
#include <ciri/wnd/Window.hpp>

//static void setVSync(bool sync)
//{	
//	// Function pointer for the wgl extention function we need to enable/disable
//	// vsync
//	typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALPROC)( int );
//	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;
//
//	const char *extensions = (char*)glGetString( GL_EXTENSIONS );
//
//	if( false && strstr( extensions, "WGL_EXT_swap_control" ) == 0 )
//	{
//		return;
//	}
//	else
//	{
//		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress( "wglSwapIntervalEXT" );
//
//		if( wglSwapIntervalEXT )
//			wglSwapIntervalEXT(sync);
//	}
//}

namespace ciri {
	GLGraphicsDevice::GLGraphicsDevice()
		: IGraphicsDevice(), _hdc(0), _hglrc(0), _defaultWidth(0), _defaultHeight(0), _activeShader(nullptr),
			_activeVertexBuffer(nullptr), _activeIndexBuffer(nullptr), _currentFbo(0) {
		// configure mrt draw buffers
		for( int i = 0; i < MAX_MRTS; ++i ) {
			_drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
		}
	}

	GLGraphicsDevice::~GLGraphicsDevice() {
		destroy();
	}

	bool GLGraphicsDevice::create( Window* window ) {
		_defaultWidth = window->getSize().x;
		_defaultHeight = window->getSize().y;

		if( !configureGl(window->getHandle()) ) {
			destroy();
			return false;
		}

		if( !configureGlew() ) {
			destroy();
			return false;
		}

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);

		return true;
	}

	void GLGraphicsDevice::destroy() {
		// clean fbo
		if( 0 != _currentFbo ) {
			glDeleteFramebuffers(1, &_currentFbo);
			_currentFbo = 0;
		}

		// destroy 2d render targets
		for( unsigned int i = 0; i < _renderTarget2Ds.size(); ++i ) {
			if( _renderTarget2Ds[i] != nullptr ) {
				_renderTarget2Ds[i]->destroy();
				delete _renderTarget2Ds[i];
				_renderTarget2Ds[i] = nullptr;
			}
		}
		_renderTarget2Ds.clear();

		// destroy samplers
		for( unsigned int i = 0; i < _samplers.size(); ++i ) {
			if( _samplers[i] != nullptr ) {
				_samplers[i]->destroy();
				delete _samplers[i];
				_samplers[i] = nullptr;
			}
		}
		_samplers.clear();

		// destroy 2d textures
		for( unsigned int i = 0; i < _texture2Ds.size(); ++i ) {
			if( _texture2Ds[i] != nullptr ) {
				_texture2Ds[i]->destroy();
				delete _texture2Ds[i];
				_texture2Ds[i] = nullptr;
			}
		}
		_texture2Ds.clear();

		// destroy constant buffers
		for( unsigned int i = 0; i < _constantBuffers.size(); ++i ) {
			if( _constantBuffers[i] != nullptr ) {
				_constantBuffers[i]->destroy();
				delete _constantBuffers[i];
				_constantBuffers[i] = nullptr;
			}
		}
		_constantBuffers.clear();

		// destroy index buffers
		for( unsigned int i = 0; i < _indexBuffers.size(); ++i ) {
			if( _indexBuffers[i] != nullptr ) {
				_indexBuffers[i]->destroy();
				delete _indexBuffers[i];
				_indexBuffers[i] = nullptr;
			}
		}
		_indexBuffers.clear();

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
		GLShader* shader = new GLShader();
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
		GLVertexBuffer* buffer = new GLVertexBuffer();
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

		_activeVertexBuffer = glBuffer;
	}

	IIndexBuffer* GLGraphicsDevice::createIndexBuffer() {
		GLIndexBuffer* buffer = new GLIndexBuffer();
		_indexBuffers.push_back(buffer);
		return buffer;
	}

	void GLGraphicsDevice::setIndexBuffer( IIndexBuffer* buffer ) {
		GLIndexBuffer* glBuffer = reinterpret_cast<GLIndexBuffer*>(buffer);

		const GLuint evbo = glBuffer->getEvbo();
		if( 0 == evbo ) {
			return; // todo: error
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, evbo);

		_activeIndexBuffer = glBuffer;
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

		glDrawArrays(convertTopology(topology), startIndex, vertexCount);
	}

	void GLGraphicsDevice::drawIndexed( PrimitiveTopology::Type topology, int indexCount ) {
		// cannot draw with no active shader
		if( nullptr == _activeShader ) {
			return;
		}

		// cannot draw without a valid vertex and index buffer
		if( nullptr == _activeVertexBuffer || nullptr == _activeIndexBuffer ) {
			return;
		}

		// index count must be greater than 0
		if( indexCount <= 0 ) {
			return; // todo: error
		}

		glDrawElements(convertTopology(topology), indexCount, GL_UNSIGNED_INT, 0);
	}

	void GLGraphicsDevice::clear( ClearFlags::Flags flags, float* color ) {
		GLbitfield clearFlags = 0;
		switch( flags ) {
			case ClearFlags::Color: {
					clearFlags = GL_COLOR_BUFFER_BIT;
				break;
			}

			case ClearFlags::Depth: {
					clearFlags = GL_DEPTH_BUFFER_BIT;
				break;
			}

			case ClearFlags::Stencil: {
					clearFlags = GL_STENCIL_BUFFER_BIT;
				break;
			}

			case ClearFlags::ColorDepth: {
					clearFlags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
				break;
			}

			case ClearFlags::ColorStencil: {
					clearFlags = GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
				break;
			}

			case ClearFlags::DepthStencil: {
					clearFlags = GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
				break;
			}

			case ClearFlags::All: {
					clearFlags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
				break;
			}
		}

		if( color != nullptr ) {
			glClearColor(color[0], color[1], color[2], color[3]);
		} else {
			glClearColor(0.39f, 0.58f, 0.93f, 1.0f);
		}
		glClear(clearFlags);
	}

	IConstantBuffer* GLGraphicsDevice::createConstantBuffer() {
		GLConstantBuffer* buffer = new GLConstantBuffer();
		_constantBuffers.push_back(buffer);
		return buffer;
	}

	ITexture2D* GLGraphicsDevice::createTexture2D() {
		GLTexture2D* glTexture = new GLTexture2D();
		_texture2Ds.push_back(glTexture);
		return glTexture;
	}

	void GLGraphicsDevice::setTexture2D( int index, ITexture2D* texture, ShaderStage::Stage shaderStage ) {
		GLTexture2D* glTexture = reinterpret_cast<GLTexture2D*>(texture);
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, (texture != nullptr) ? glTexture->getTextureId() : 0);
	}

	ISamplerState* GLGraphicsDevice::createSamplerState( const SamplerDesc& desc ) {
		GLSamplerState* glSampler = new GLSamplerState();
		if( !glSampler->create(desc) ) {
			delete glSampler;
			glSampler = nullptr;
			return nullptr;
		}
		_samplers.push_back(glSampler);
		return glSampler;
	}

	void GLGraphicsDevice::setSamplerState( int index, ISamplerState* state, ShaderStage::Stage shaderStage ) {
		GLSamplerState* glSampler = reinterpret_cast<GLSamplerState*>(state);
		glBindSampler(index, (state != nullptr) ? glSampler->getSamplerId() : 0);
	}

	IRenderTarget2D* GLGraphicsDevice::createRenderTarget2D( int width, int height, TextureFormat::Type format ) {
		GLTexture2D* texture = reinterpret_cast<GLTexture2D*>(this->createTexture2D());
		if( !texture->setData(0, 0, width, height, nullptr, format) ) {
			texture->destroy();
			delete texture;
			texture = nullptr;
			_texture2Ds.pop_back();
		}
		GLRenderTarget2D* glTarget = new GLRenderTarget2D(texture);
		_renderTarget2Ds.push_back(glTarget);
		return glTarget;
	}

	void GLGraphicsDevice::setRenderTargets( IRenderTarget2D** renderTargets, int numRenderTargets ) {
		// create and bind a framebuffer
		if( 0 == _currentFbo ) { glGenFramebuffers(1, &_currentFbo); }
		glBindFramebuffer(GL_FRAMEBUFFER, _currentFbo);

		// attach all render target textures
		for( int i = 0; i < numRenderTargets; ++i ) {
			GLTexture2D* texture = reinterpret_cast<GLTexture2D*>(renderTargets[i]->getTexture2D());
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture->getTextureId(), 0);
		}

		// configure draw buffers
		glDrawBuffers(numRenderTargets, _drawBuffers);

		// set viewport (use 0's size)
		glViewport(0, 0, renderTargets[0]->getTexture2D()->getWidth(), renderTargets[0]->getTexture2D()->getHeight());
	}

	void GLGraphicsDevice::restoreDefaultRenderTargets() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, _defaultWidth, _defaultHeight);
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

		glViewport(0, 0, _defaultWidth, _defaultHeight);

		//TODO: setup gl debug message tracing

		return true;
	}

	bool GLGraphicsDevice::configureGlew() {
		return (glewInit() == GLEW_OK);
	}

	GLenum GLGraphicsDevice::convertTopology( PrimitiveTopology::Type topology ) const {
		switch( topology ) {
			case PrimitiveTopology::PointList: {
				return GL_POINTS;
			}

			case PrimitiveTopology::LineList: {
				return GL_LINES; // wrong?
			}

			case PrimitiveTopology::LineStrip: {
				return GL_LINE_STRIP;
			}

			case PrimitiveTopology::TriangleList: {
				return GL_TRIANGLES; // wrong?
			}

			case PrimitiveTopology::TriangleStrip: {
				return GL_TRIANGLE_STRIP;
			}

			default: {
				return GL_INVALID_ENUM; // lol
			}
		}
	}
} // ciri