#include <ciri/gfx/gl/GLGraphicsDevice.hpp>
#include <ciri/wnd/Window.hpp>
#include <sstream>
#include <ciri/gfx/gl/CiriToGl.hpp>

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
		: IGraphicsDevice(), _isValid(false), _window(nullptr), _hdc(0), _hglrc(0), _defaultWidth(0), _defaultHeight(0), _activeShader(nullptr),
			_activeVertexBuffer(nullptr), _activeIndexBuffer(nullptr), _currentFbo(0), _activeRasterizerState(nullptr),
			_activeDepthStencilState(nullptr), _shaderExt(".glsl") {
		// configure mrt draw buffers
		for( int i = 0; i < MAX_MRTS; ++i ) {
			_drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
		}
	}

	GLGraphicsDevice::~GLGraphicsDevice() {
		destroy();
	}

	bool GLGraphicsDevice::create( Window* window ) {
		if( _isValid ) {
			return false;
		}

		_window = window;

		_defaultWidth = window->getSize().x;
		_defaultHeight = window->getSize().y;

		if( !configureGlew() ) {
			destroy();
			return false;
		}

		if( !configureGl(window->getHandle()) ) {
			destroy();
			return false;
		}

		// temp:
		//printf("**********\n");
		//printf("Vendor: %s\n", glGetString(GL_VENDOR));
		//printf("Renderer: %s\n", glGetString(GL_RENDERER));
		//printf("Version: %s\n", glGetString(GL_VERSION));
		//printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
		//printf("**********\n");

		_isValid = true;

		return true;
	}

	void GLGraphicsDevice::destroy() {
		if( !_isValid ) {
			return;
		}

		// clean depth stencil states
		for( unsigned int i = 0; i < _depthStencilStates.size(); ++i ) {
			if( _depthStencilStates[i] != nullptr ) {
				_depthStencilStates[i]->destroy();
				delete _depthStencilStates[i];
				_depthStencilStates[i] = nullptr;
			}
		}
		_depthStencilStates.clear();

		// clean rasterizer states
		for( unsigned int i = 0; i < _rasterizerStates.size(); ++i ) {
			if( _rasterizerStates[i] != nullptr ) {
				_rasterizerStates[i]->destroy();
				delete _rasterizerStates[i];
				_rasterizerStates[i] = nullptr;
			}
		}
		_rasterizerStates.clear();

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

		_isValid = false;
	}

	void GLGraphicsDevice::present() {
		if( !_isValid ) {
			return;
		}

		SwapBuffers(_hdc);
	}

	IShader* GLGraphicsDevice::createShader() {
		if( !_isValid ) {
			return nullptr;
		}

		GLShader* shader = new GLShader();
		_shaders.push_back(shader);
		return shader;
	}

	IVertexBuffer* GLGraphicsDevice::createVertexBuffer() {
		if( !_isValid ) {
			return nullptr;
		}

		GLVertexBuffer* buffer = new GLVertexBuffer();
		_vertexBuffers.push_back(buffer);
		return buffer;
	}

	IIndexBuffer* GLGraphicsDevice::createIndexBuffer() {
		if( !_isValid ) {
			return nullptr;
		}

		GLIndexBuffer* buffer = new GLIndexBuffer();
		_indexBuffers.push_back(buffer);
		return buffer;
	}

	IConstantBuffer* GLGraphicsDevice::createConstantBuffer() {
		if( !_isValid ) {
			return nullptr;
		}

		// note: using the size of a vector as the index means they can't be removed.
		// todo: update the index to a smarter system that allows for reuse of deleted buffer indices
		GLConstantBuffer* buffer = new GLConstantBuffer(_constantBuffers.size());
		_constantBuffers.push_back(buffer);
		return buffer;
	}

	ITexture2D* GLGraphicsDevice::createTexture2D( int width, int height, TextureFormat::Format format, int flags, void* pixels ) {
		if( !_isValid ) {
			return nullptr;
		}

		if( width <= 0 || height <= 0 ) {
			return nullptr;
		}

		GLTexture2D* glTexture = new GLTexture2D(flags);
		if( err::failed(glTexture->setData(0, 0, width, height, pixels, format)) ) {
			delete glTexture;
			glTexture = nullptr;
			return nullptr;
		}

		_texture2Ds.push_back(glTexture);
		return glTexture;
	}

	ISamplerState* GLGraphicsDevice::createSamplerState( const SamplerDesc& desc ) {
		if( !_isValid ) {
			return nullptr;
		}

		GLSamplerState* glSampler = new GLSamplerState();
		if( !glSampler->create(desc) ) {
			delete glSampler;
			glSampler = nullptr;
			return nullptr;
		}
		_samplers.push_back(glSampler);
		return glSampler;
	}

	IRenderTarget2D* GLGraphicsDevice::createRenderTarget2D( int width, int height, TextureFormat::Format format ) {
		if( !_isValid ) {
			return nullptr;
		}

		GLTexture2D* texture = reinterpret_cast<GLTexture2D*>(this->createTexture2D(width, height, format, TextureFlags::RenderTarget, nullptr));
		if( nullptr == texture ) {
			return nullptr;
		}
		GLRenderTarget2D* glTarget = new GLRenderTarget2D(texture);
		_renderTarget2Ds.push_back(glTarget);
		return glTarget;
	}

	IRasterizerState* GLGraphicsDevice::createRasterizerState( const RasterizerDesc& desc ) {
		if( !_isValid ) {
			return nullptr;
		}

		GLRasterizerState* glRaster = new GLRasterizerState();
		if( !glRaster->create(desc) ) {
			delete glRaster;
			glRaster = nullptr;
			return nullptr;
		}
		_rasterizerStates.push_back(glRaster);
		return glRaster;
	}

	IDepthStencilState* GLGraphicsDevice::createDepthStencilState( const DepthStencilDesc& desc ) {
		if( !_isValid ) {
			return nullptr;
		}

		GLDepthStencilState* glState = new GLDepthStencilState();
		if( !glState->create(desc) ) {
			delete glState;
			glState = nullptr;
			return nullptr;
		}
		_depthStencilStates.push_back(glState);
		return glState;
	}

	void GLGraphicsDevice::applyShader( IShader* shader ) {
		if( !_isValid ) {
			return;
		}

		if( !shader->isValid() ) {
			_activeShader = nullptr;
			return;
		}

		GLShader* glShader = reinterpret_cast<GLShader*>(shader);
		glUseProgram(glShader->getProgram());
		_activeShader = glShader;
	}

	void GLGraphicsDevice::setVertexBuffer( IVertexBuffer* buffer ) {
		if( !_isValid ) {
			return;
		}

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

			GLenum type;
			switch( currElement.getFormat() ) {
				case VertexFormat::Float:
				case VertexFormat::Float2:
				case VertexFormat::Float3:
				case VertexFormat::Float4: {
					type = GL_FLOAT;
				}
			}

			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, currElement.getMultiplicity(), type, GL_FALSE, _activeShader->getVertexDeclaration().getStride(), (const void*)offset);
			
			offset += currElement.getSize(); // sizeof(datatype) * numberOfThem;
		}

		_activeVertexBuffer = glBuffer;
	}

	void GLGraphicsDevice::setIndexBuffer( IIndexBuffer* buffer ) {
		if( !_isValid ) {
			return;
		}

		GLIndexBuffer* glBuffer = reinterpret_cast<GLIndexBuffer*>(buffer);

		const GLuint evbo = glBuffer->getEvbo();
		if( 0 == evbo ) {
			return; // todo: error
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, evbo);

		_activeIndexBuffer = glBuffer;
	}

	void GLGraphicsDevice::setTexture2D( int index, ITexture2D* texture, ShaderStage::Stage shaderStage ) {
		if( !_isValid ) {
			return;
		}

		GLTexture2D* glTexture = reinterpret_cast<GLTexture2D*>(texture);
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, (texture != nullptr) ? glTexture->getTextureId() : 0);
	}

	void GLGraphicsDevice::setSamplerState( int index, ISamplerState* state, ShaderStage::Stage shaderStage ) {
		if( !_isValid ) {
			return;
		}

		GLSamplerState* glSampler = reinterpret_cast<GLSamplerState*>(state);
		glBindSampler(index, (state != nullptr) ? glSampler->getSamplerId() : 0);
	}

	void GLGraphicsDevice::drawArrays( PrimitiveTopology::Topology topology, int vertexCount, int startIndex ) {
		if( !_isValid ) {
			return;
		}

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

		glDrawArrays(ciriToGlTopology(topology), startIndex, vertexCount);
	}

	void GLGraphicsDevice::drawIndexed( PrimitiveTopology::Topology topology, int indexCount ) {
		if( !_isValid ) {
			return;
		}

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

		glDrawElements(ciriToGlTopology(topology), indexCount, GL_UNSIGNED_INT, 0);
	}

	void GLGraphicsDevice::setRenderTargets( IRenderTarget2D** renderTargets, int numRenderTargets ) {
		if( !_isValid ) {
			return;
		}

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
		if( !_isValid ) {
			return;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, _defaultWidth, _defaultHeight);
	}

	err::ErrorCode GLGraphicsDevice::resize() {
		if( !_isValid ) {
			return err::CIRI_UNKNOWN_ERROR;
		}

		// todo: check for erroneous sizes

		// get new window size
		const int width = _window->getSize().x;
		const int height = _window->getSize().y;

		// don't resize if the same size
		if( width == _defaultWidth && height == _defaultHeight ) {
			return err::CIRI_OK; // not incorrect; just ignore it
		}

		// update default width and height
		_defaultWidth = width;
		_defaultHeight = height;
		
		// bind backbuffer and resize the viewport
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, _defaultWidth, _defaultHeight);

		return err::CIRI_OK;
	}

	void GLGraphicsDevice::setClearColor( float r, float g, float b, float a ) {
		if( !_isValid ) {
			return;
		}

		glClearColor(r, g, b, a);
	}

	void GLGraphicsDevice::clear( int flags ) {
		if( !_isValid ) {
			return;
		}

		GLbitfield clearFlags = 0;
		if( flags & ClearFlags::Color ) {
			clearFlags |= GL_COLOR_BUFFER_BIT;
		}
		if( flags & ClearFlags::Depth ) {
			clearFlags |= GL_DEPTH_BUFFER_BIT;
		}
		if( flags & ClearFlags::Stencil ) {
			clearFlags |= GL_STENCIL_BUFFER_BIT;
		}
		glClear(clearFlags);
	}

	void GLGraphicsDevice::setRasterizerState( IRasterizerState* state ) {
		if( !_isValid ) {
			return;
		}

		// todo: if state is nullptr, revert to a default set state
		if( nullptr == state ) {
			throw; // not yet implemented
		}
		if( state == _activeRasterizerState ) {
			return;
		}
		_activeRasterizerState = state;

		GLRasterizerState* glRaster = reinterpret_cast<GLRasterizerState*>(state);
		const RasterizerDesc& desc = glRaster->getDesc();

		// cull mode
		if( CullMode::None == desc.cullMode ) {
			glDisable(GL_CULL_FACE);
		} else {
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			if( CullMode::Clockwise == desc.cullMode ) {
				glFrontFace(GL_CCW);
			} else {
				glFrontFace(GL_CW);
			}
		}
		// fill mode
		if( FillMode::Solid == desc.fillMode ) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		// scissor test
		if( desc.scissorTestEnable ) {
			glEnable(GL_SCISSOR_TEST);
		} else {
			glDisable(GL_SCISSOR_TEST);
		}
		// depth bias and slope scale depth bioas
		if( desc.depthBias != 0.0f || desc.slopeScaleDepthBias != 0.0f ) {
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(desc.slopeScaleDepthBias, desc.depthBias);
		} else {
			glDisable(GL_POLYGON_OFFSET_FILL);
		}
		// todo: only if depth clamping is supported (how?)
		// depth clipping
		if( !desc.depthClipEnable ) {
			glEnable(GL_DEPTH_CLAMP);
		} else {
			glDisable(GL_DEPTH_CLAMP);
		}
		// todo: msaa
	}

	void GLGraphicsDevice::setDepthStencilState( IDepthStencilState* state ) {
		if( !_isValid ) {
			return;
		}

		// todo: if state is nullptr, revert to a default set state
		if( nullptr == state ) {
			throw; // not yet implemented
		}
		if( state == _activeDepthStencilState ) {
			return;
		}
		_activeDepthStencilState = state;

		GLDepthStencilState* glState = reinterpret_cast<GLDepthStencilState*>(state);
		const DepthStencilDesc& desc = glState->getDesc();

		// enable depth
		if( desc.depthEnable ) {
			glEnable(GL_DEPTH_TEST);
		} else {
			glDisable(GL_DEPTH_TEST);
		}

		// depth func
		glDepthFunc(ciriToGlComparisonFunc(desc.depthFunc));

		// depth write
		glDepthMask(desc.depthWriteMask);

		// enable stencil
		if( desc.stencilEnable ) {
			glEnable(GL_STENCIL_TEST);
		} else {
			glDisable(GL_STENCIL_TEST);
		}

		// stencil funcs and ops
		if( desc.twoSidedStencil ) {
			glStencilFuncSeparate(GL_FRONT, ciriToGlComparisonFunc(desc.frontStencilFunc), desc.stencilRef, desc.stencilReadMask);
			glStencilFuncSeparate(GL_BACK, ciriToGlComparisonFunc(desc.backStencilFunc), desc.stencilRef, desc.stencilReadMask);
			glStencilOpSeparate(GL_FRONT, ciriToGlStencilOp(desc.frontStencilFailOp), ciriToGlStencilOp(desc.frontStencilDepthFailOp), ciriToGlStencilOp(desc.frontStencilPassOp));
			glStencilOpSeparate(GL_BACK, ciriToGlStencilOp(desc.backStencilFailOp), ciriToGlStencilOp(desc.backStencilDepthFailOp), ciriToGlStencilOp(desc.backStencilPassOp));
		} else {
			glStencilFunc(ciriToGlComparisonFunc(desc.frontStencilFunc), desc.stencilRef, desc.stencilReadMask);
			glStencilOp(ciriToGlStencilOp(desc.frontStencilFailOp), ciriToGlStencilOp(desc.frontStencilDepthFailOp), ciriToGlStencilOp(desc.frontStencilPassOp));
		}

		// stencil write mask
		glStencilMask(desc.stencilWriteMask);
	}

	void GLGraphicsDevice::setShaderExt( const char* ext ) {
		_shaderExt = ext;
	}

	const char* GLGraphicsDevice::getShaderExt() const {
		return _shaderExt.c_str();
	}

	Window* GLGraphicsDevice::getWindow() const {
		return _window;
	}

	const char* GLGraphicsDevice::getGpuName() const {
		return _gpuName.c_str();
	}

	const char* GLGraphicsDevice::getApiInfo() const {
		return _apiInfo.c_str();
	}

	bool GLGraphicsDevice::configureGl( HWND hwnd ) {
		// get the window's device context
		_hdc = GetDC(hwnd);

		// choose the pixel format
		const int pixelFormatAttribs[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			WGL_SAMPLE_BUFFERS_ARB, 1,
			WGL_SAMPLES_ARB, 1,
			0
		};
		int pixelFormat;
		UINT numPixelFormats;
		if( !wglChoosePixelFormatARB(_hdc, pixelFormatAttribs, NULL, 1, &pixelFormat, &numPixelFormats) ) {
			return false;
		}

		// set the pixel format (pfd is redundant
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		if( !SetPixelFormat(_hdc, pixelFormat, &pfd) ) {
			return false;
		}

		// create the newer opengl context
		int contextFlags = 0;// WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
		#ifdef _DEBUG
			contextFlags |= WGL_CONTEXT_DEBUG_BIT_ARB;
		#endif
		const int contextAttribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3,
			WGL_CONTEXT_FLAGS_ARB, contextFlags,
			//WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
			0
		};
		_hglrc = wglCreateContextAttribsARB(_hdc, NULL, contextAttribs);
		if( !_hglrc ) {
			return false;
		}

		// make the new context current
		if( !wglMakeCurrent(_hdc, _hglrc) ) {
			return false;
		}

		// configure opengl debug output
		#ifdef _DEBUG
		if( glDebugMessageCallback != NULL ) {
			glDebugMessageCallback(debugContextCb, this);
		}
		if( glDebugMessageCallbackARB != NULL ) {
			glDebugMessageCallbackARB(debugContextCb, this);
		}
		if( glDebugMessageCallbackAMD != NULL ) {
			glDebugMessageCallbackAMD(debugContextAmdCb, this);
		}
		#endif

		// get gpu information
		const std::string vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
		const std::string renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
		_gpuName = vendor + " " + renderer;
		const std::string glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
		const std::string glslVersion = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
		_apiInfo = "OpenGL " + glVersion + "; GLSL " + glslVersion;

		// default to fullscreen viewport
		glViewport(0, 0, _defaultWidth, _defaultHeight);

		// default clear color
		glClearColor(0.39f, 0.58f, 0.93f, 1.0f);

		return true;
	}

	// i shit you the fuck not, we even need a fake wndproc because of the SetPixelFormat thing mentioned below...
	static LRESULT WINAPI FakeWndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam ) {
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	bool GLGraphicsDevice::configureGlew() {
		// note: because Microsoft APIs are so nice to work with, we must create a fake window to initialize a fake GL context and GLEW because you can't set the pixel format more than once

		// create a dummy window
		const char* className = "CIRI_DUMMY";

		// create a dummy window
		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = (WNDPROC)FakeWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetModuleHandle(NULL);
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = className;
		if( !RegisterClassEx(&wc) ) {
			return false;
		}
		HINSTANCE inst = GetModuleHandle(NULL);
		HWND hwnd = CreateWindow(className, className, WS_OVERLAPPEDWINDOW, 0, 0, CW_USEDEFAULT, CW_DEFAULT, NULL, NULL, GetModuleHandle(NULL), NULL);

		// get window's HDC
		HDC hdc = GetDC(hwnd);

		// choose a pixel format
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;
		pfd.iLayerType = PFD_MAIN_PLANE;
		const int pixelFormat = ChoosePixelFormat(hdc, &pfd);
		if( 0 == pixelFormat ) {
			DestroyWindow(hwnd);
			return false;
		}
		if( !SetPixelFormat(hdc, pixelFormat, &pfd) ) {
			return false;
		}

		// create an opengl 2.1 or older context
		HGLRC fakeHglrc = wglCreateContext(hdc);
		if( !fakeHglrc ) {
			DestroyWindow(hwnd);
			return false;
		}
		if( !wglMakeCurrent(hdc, fakeHglrc) ) {
			DestroyWindow(hwnd);
			return false;
		}

		// initialize GLEW
		if( glewInit() != GLEW_OK ) {
			DestroyWindow(hwnd);
			return false;
		}

		// delete the fake context and window
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(fakeHglrc);
		DeleteDC(hdc);
		DestroyWindow(hwnd);

		return true;
	}

	void APIENTRY GLGraphicsDevice::debugContextCb( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam ) {
		const bool SHOULD_IGNORE_NOTIFICATIONS = true;
		if( GL_DEBUG_SEVERITY_NOTIFICATION == severity && SHOULD_IGNORE_NOTIFICATIONS ) {
			return;
		}

		std::stringstream ss;
		switch( severity ) {
			case GL_DEBUG_SEVERITY_HIGH: {
				ss << "OpenGL Error (HIGH): ";
				break;
			}
			case GL_DEBUG_SEVERITY_MEDIUM: {
				ss << "OpenGL Error (MEDIUM): ";
				break;
			}
			case GL_DEBUG_SEVERITY_LOW: {
				ss << "OpenGL Error (LOW): ";
				break;
			}
			case GL_DEBUG_SEVERITY_NOTIFICATION: {
				ss << "OpenGL Notification: ";
				break;
			}
			default: {
				break;
			}
		}
		ss << message << "\n";
		OutputDebugString(ss.str().c_str());
		printf(ss.str().c_str());
	}

	void APIENTRY GLGraphicsDevice::debugContextAmdCb( GLuint id, GLenum category, GLenum severity, GLsizei length, const GLchar* message, void* userParam ) {
		std::stringstream ss;
		switch( severity ) {
			case GL_DEBUG_SEVERITY_HIGH_AMD: {
					ss << "OpenGL Error (HIGH): ";
				break;
			}
			case GL_DEBUG_SEVERITY_MEDIUM_AMD: {
					ss << "OpenGL Error (MEDIUM): ";
				break;
			}
			case GL_DEBUG_SEVERITY_LOW_AMD: {
					ss << "OpenGL Error (LOW): ";
				break;
			}
			case GL_DEBUG_SEVERITY_NOTIFICATION: {
					ss << "OpenGL Notification: ";
				break;
			}
			default: {
				break;
			}
		}
		ss << message << "\n";
		OutputDebugString(ss.str().c_str());
		printf(ss.str().c_str());
	}
} // ciri
