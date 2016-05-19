#include <sstream>
#include <ciri/graphics/win/gl/GLGraphicsDevice.hpp>
#include <ciri/core/window/IWindow.hpp>
#include <ciri/graphics/win/gl/GLConstantBuffer.hpp>
#include <ciri/graphics/win/gl/GLTexture2D.hpp>
#include <ciri/graphics/win/gl/GLTexture3D.hpp>
#include <ciri/graphics/win/gl/GLTextureCube.hpp>
#include <ciri/graphics/win/gl/GLSamplerState.hpp>
#include <ciri/graphics/win/gl/GLRenderTarget2D.hpp>
#include <ciri/graphics/win/gl/GLBlendState.hpp>
#include <ciri/graphics/win/gl/CiriToGl.hpp>
#include <ciri/graphics/ClearFlags.hpp>

using namespace ciri::graphics;
using namespace ciri::core;

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

GLGraphicsDevice::GLGraphicsDevice()
	: IGraphicsDevice(), _isValid(false), _window(nullptr), _hdc(0), _hglrc(0), _defaultWidth(0), _defaultHeight(0),
		_currentFbo(0), _shaderExt(".glsl"), _dummyVao(0), _constantBufferCount(0) {
	// configure mrt draw buffers
	for( int i = 0; i < MAX_MRTS; ++i ) {
		_drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}
}

GLGraphicsDevice::~GLGraphicsDevice() {
	destroy();
}

bool GLGraphicsDevice::create( const std::shared_ptr<IWindow>& window ) {
	if( _isValid ) {
		return false;
	}

	_window = window;

	_defaultWidth = window->getWidth();
	_defaultHeight = window->getHeight();

	if( !configureGlew() ) {
		destroy();
		return false;
	}

	if( !configureGl(static_cast<HWND>(window->getNativeHandle())) ) {
		destroy();
		return false;
	}

	_isValid = true;

	// apply a fullsize viewport
	setViewport(Viewport(0, 0, window->getWidth(), window->getHeight(), 0.0f, 1.0f));

	// set default states
	restoreDefaultStates();

	return true;
}

void GLGraphicsDevice::destroy() {
	if( !_isValid ) {
		return;
	}

	// clean default state pointers
	_defaultBlendAdditive = nullptr;
	_defaultBlendAlpha = nullptr;
	_defaultBlendNonPremul = nullptr;
	_defaultBlendOpaque = nullptr;
	_defaultRasterNone = nullptr;
	_defaultRasterClockwise = nullptr;
	_defaultRasterCounterClockwise = nullptr;
	_defaultDepthStencilDefault = nullptr;
	_defaultDepthStencilDepthRead = nullptr;
	_defaultDepthStencilNone = nullptr;

	// clean fbo
	if( 0 != _currentFbo ) {
		glDeleteFramebuffers(1, &_currentFbo);
		_currentFbo = 0;
	}

	// delete dummy vao
	if( _dummyVao != 0 ) {
		glDeleteVertexArrays(1, &_dummyVao);
		_dummyVao = 0;
	}

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

void GLGraphicsDevice::setViewport( const Viewport& vp ) {
	// todo: if has valid context, can apply

	glViewport(vp.x(), vp.y(), vp.width(), vp.height());
	glDepthRange(vp.minDepth(), vp.maxDepth());

	_activeViewport = vp;
}

const Viewport& GLGraphicsDevice::getViewport() const {
	return _activeViewport;
}

std::shared_ptr<IShader> GLGraphicsDevice::createShader() {
	if( !_isValid ) {
		return nullptr;
	}

	std::shared_ptr<GLShader> shader = std::make_shared<GLShader>();
	return shader;
}

std::shared_ptr<IVertexBuffer> GLGraphicsDevice::createVertexBuffer() {
	if( !_isValid ) {
		return nullptr;
	}
	std::shared_ptr<GLVertexBuffer> buffer = std::make_shared<GLVertexBuffer>();
	return buffer;
}

std::shared_ptr<IIndexBuffer> GLGraphicsDevice::createIndexBuffer() {
	if( !_isValid ) {
		return nullptr;
	}

	std::shared_ptr<GLIndexBuffer> buffer = std::make_shared<GLIndexBuffer>();
	return buffer;
}

std::shared_ptr<IConstantBuffer> GLGraphicsDevice::createConstantBuffer() {
	if( !_isValid ) {
		return nullptr;
	}

	// note: using a hardcoded index means they cannot be removed.
	// todo: implement a better system that allows for reuse of deleted indices
	//       such as some kind of lookup table to see if a buffer is dead upon request.
	//       e.g. .....(getNextFreeConstantBufferIndex())...
	std::shared_ptr<GLConstantBuffer> buffer = std::make_shared<GLConstantBuffer>(_constantBufferCount);
	_constantBufferCount += 1;
	return buffer;
}

std::shared_ptr<ITexture2D> GLGraphicsDevice::createTexture2D( int width, int height, TextureFormat::Format format, int flags, void* pixels ) {
	if( !_isValid ) {
		return nullptr;
	}

	if( width <= 0 || height <= 0 ) {
		return nullptr;
	}

	std::shared_ptr<GLTexture2D> glTexture = std::make_shared<GLTexture2D>(flags);
	if( failed(glTexture->setData(0, 0, width, height, pixels, format)) ) {
		glTexture.reset();
		glTexture = nullptr;
		return nullptr;
	}

	return glTexture;
}

std::shared_ptr<ITexture3D> GLGraphicsDevice::createTexture3D( int width, int height, int depth, TextureFormat::Format format, int flags, void* pixels ) {
	if( !_isValid ) {
		return nullptr;
	}

	if( width <= 0 || height <= 0 || depth <= 0 ) {
		return nullptr;
	}

	std::shared_ptr<GLTexture3D> glTexture = std::make_shared<GLTexture3D>(flags);
	if( failed(glTexture->setData(width, height, depth, pixels, format)) ) {
		glTexture.reset();
		glTexture = nullptr;
		return nullptr;
	}

	return glTexture;
}


std::shared_ptr<ITextureCube> GLGraphicsDevice::createTextureCube( int width, int height, void* posx, void* negx, void* posy, void* negy, void* posz, void* negz ) {
	if( width <= 0 || height <= 0 ) {
		return nullptr;
	}

	if( nullptr==posx || nullptr==negx || nullptr==posy || nullptr==negy || nullptr==posz || nullptr==negz ) {
		return nullptr;
	}

	std::shared_ptr<GLTextureCube> glCube = std::make_shared<GLTextureCube>();
	if( failed(glCube->set(width, height, posx, negx, posy, negy, posz, negz)) ) {
		glCube.reset();
		glCube = nullptr;
		return nullptr;
	}

	return glCube;
}

std::shared_ptr<ISamplerState> GLGraphicsDevice::createSamplerState( const SamplerDesc& desc ) {
	if( !_isValid ) {
		return nullptr;
	}

	std::shared_ptr<GLSamplerState> glSampler = std::make_shared<GLSamplerState>();
	if( !glSampler->create(desc) ) {
		glSampler.reset();
		glSampler = nullptr;
		return nullptr;
	}
	return glSampler;
}

std::shared_ptr<IRenderTarget2D> GLGraphicsDevice::createRenderTarget2D( int width, int height, TextureFormat::Format format ) {
	if( !_isValid ) {
		return nullptr;
	}

	std::shared_ptr<GLTexture2D> texture = std::static_pointer_cast<GLTexture2D>(this->createTexture2D(width, height, format, TextureFlags::RenderTarget, nullptr));
	if( nullptr == texture ) {
		return nullptr;
	}
	std::shared_ptr<GLRenderTarget2D> glTarget = std::make_shared<GLRenderTarget2D>();
	if( !glTarget->create(texture) ) {
		texture.reset();
		return nullptr;
	}
	return glTarget;
}

std::shared_ptr<IRasterizerState> GLGraphicsDevice::createRasterizerState( const RasterizerDesc& desc ) {
	if( !_isValid ) {
		return nullptr;
	}

	std::shared_ptr<GLRasterizerState> glRaster = std::make_shared<GLRasterizerState>();
	if( !glRaster->create(desc) ) {
		glRaster.reset();
		glRaster = nullptr;
		return nullptr;
	}
	return glRaster;
}

std::shared_ptr<IDepthStencilState> GLGraphicsDevice::createDepthStencilState( const DepthStencilDesc& desc ) {
	if( !_isValid ) {
		return nullptr;
	}

	std::shared_ptr<GLDepthStencilState> glState = std::make_shared<GLDepthStencilState>();
	if( !glState->create(desc) ) {
		glState.reset();
		glState = nullptr;
		return nullptr;
	}
	return glState;
}

std::shared_ptr<IBlendState> GLGraphicsDevice::createBlendState( const BlendDesc& desc ) {
	if( !_isValid ) {
		return nullptr;
	}

	std::shared_ptr<GLBlendState> glState = std::make_shared<GLBlendState>();
	if( !glState->create(desc) ) {
		glState.reset();
		glState = nullptr;
		return nullptr;
	}
	return glState;
}

void GLGraphicsDevice::applyShader( const std::shared_ptr<IShader>& shader ) {
	if( !_isValid ) {
		return;
	}

	if( nullptr == shader ) {
		glUseProgram(0);
		_activeShader.reset();
		return;
	}

	if( !shader->isValid() ) {
		_activeShader.reset();
		return;
	}

	const std::shared_ptr<GLShader> glShader = std::static_pointer_cast<GLShader>(shader);
	glUseProgram(glShader->getProgram());
	_activeShader = glShader;
}

void GLGraphicsDevice::setVertexBuffer( const std::shared_ptr<IVertexBuffer>& buffer ) {
	if( !_isValid ) {
		return;
	}

	// todo: see if there's a way to have the vertex attribute values stores separately from the buffers themselves akin
	//       to directx's input layouts.  maybe through an extension or something core to 4.2+ ?

	// gl needs a bound vertex buffer in order to set vertex attrib pointers, therefore we need a valid shader bound.
	// i want to change this because it means you can't set a vertex buffer before setting a shader, which is odd...
	if( _activeShader.expired() ) {
		return;
	}

	if( nullptr == buffer ) {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return;
	}

	const std::shared_ptr<GLVertexBuffer> glBuffer = std::static_pointer_cast<GLVertexBuffer>(buffer);
	const GLuint vbo = glBuffer->getVbo();
	if( 0 == vbo ) {
		return;
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// apply vertex attribute pointers based on the shader's vertex declaration
	int offset = 0;
	const std::vector<VertexElement>& elements = _activeShader.lock()->getVertexDeclaration().getElements();
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
		glVertexAttribPointer(i, currElement.getMultiplicity(), type, GL_FALSE, _activeShader.lock()->getVertexDeclaration().getStride(), (const void*)offset);
			
		offset += currElement.getSize(); // sizeof(datatype) * numberOfThem;
	}

	_activeVertexBuffer = glBuffer;
}

void GLGraphicsDevice::setIndexBuffer( const std::shared_ptr<IIndexBuffer>& buffer ) {
	if( !_isValid ) {
		return;
	}

	if( nullptr == buffer ) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		return;
	}

	const std::shared_ptr<GLIndexBuffer> glBuffer = std::static_pointer_cast<GLIndexBuffer>(buffer);
	const GLuint evbo = glBuffer->getEvbo();
	if( 0 == evbo ) {
		//_activeIndexBuffer.reset();
		return; // todo: error
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, evbo);

	_activeIndexBuffer = glBuffer;
}

void GLGraphicsDevice::setTexture2D( int index, const std::shared_ptr<ITexture2D>& texture, ShaderStage::Stage shaderStage ) {
	if( !_isValid ) {
		return;
	}

	if( index < 0 ) {
		return;
	}

	const std::shared_ptr<GLTexture2D> glTexture = std::static_pointer_cast<GLTexture2D>(texture);
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, (texture != nullptr) ? glTexture->getTextureId() : 0);
}

void GLGraphicsDevice::setTexture3D( int index, const std::shared_ptr<ITexture3D>& texture, ShaderStage::Stage shaderStage ) {
	if( !_isValid ) {
		return;
	}

	if( index < 0 ) {
		return;
	}

	const std::shared_ptr<GLTexture3D> glTexture = std::static_pointer_cast<GLTexture3D>(texture);
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_3D, (texture != nullptr) ? glTexture->getTextureId() : 0);
}

void GLGraphicsDevice::setTextureCube( int index, const std::shared_ptr<ITextureCube>& texture, ShaderStage::Stage shaderStage ) {
	if( !_isValid ) {
		return;
	}

	const std::shared_ptr<GLTextureCube> glTexture = std::static_pointer_cast<GLTextureCube>(texture);
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_CUBE_MAP, (glTexture != nullptr) ? glTexture->getTextureId() : 0);
}

void GLGraphicsDevice::setSamplerState( int index, const std::shared_ptr<ISamplerState>& state, ShaderStage::Stage shaderStage ) {
	if( !_isValid ) {
		return;
	}

	const std::shared_ptr<GLSamplerState> glSampler = std::static_pointer_cast<GLSamplerState>(state);
	glBindSampler(index, (state != nullptr) ? glSampler->getSamplerId() : 0);
}

void GLGraphicsDevice::setBlendState( const std::shared_ptr<IBlendState>& state ) {
	if( !_isValid ) {
		return;
	}

	if( nullptr == state ) {
		restoreDefaultBlendState();
		return;
	}

	const std::shared_ptr<GLBlendState> glState = std::static_pointer_cast<GLBlendState>(state);
	const BlendDesc desc = glState->getDesc();

	// enable blending
	if( desc.blendingEnabled() ) {
		glEnable(GL_BLEND);
	} else {
		glDisable(GL_BLEND);
	}

	// blend color
	glBlendColor(desc.blendFactor[0], desc.blendFactor[1], desc.blendFactor[2], desc.blendFactor[3]);

	// blend equations
	glBlendEquationSeparate(ciriToGlBlendFunction(desc.colorFunc), ciriToGlBlendFunction(desc.alphaFunc));

	// blend functions
	glBlendFuncSeparate(ciriToGlBlendMode(desc.srcColorBlend, false), ciriToGlBlendMode(desc.dstColorBlend, false), ciriToGlBlendMode(desc.srcAlphaBlend, true), ciriToGlBlendMode(desc.dstAlphaBlend, true));

	// color write mask
	const GLboolean redMask   = (desc.colorMask & static_cast<int>(BlendColorMask::Red)) != 0;
	const GLboolean greenMask = (desc.colorMask & static_cast<int>(BlendColorMask::Green)) != 0;
	const GLboolean blueMask  = (desc.colorMask & static_cast<int>(BlendColorMask::Blue)) != 0;
	const GLboolean alphaMask = (desc.colorMask & static_cast<int>(BlendColorMask::Alpha)) != 0;
	glColorMask(redMask, greenMask, blueMask, alphaMask);
}

void GLGraphicsDevice::drawArrays( PrimitiveTopology topology, int vertexCount, int startIndex ) {
	if( !_isValid ) {
		return;
	}

	// cannot draw with no active shader
	if( _activeShader.expired() ) {
		return;
	}

	// cannot draw with no active vertex buffer
	if( _activeVertexBuffer.expired() ) {
		return;
	}

	// vertex count must be greater than 0!
	if( vertexCount <= 0 ) {
		return;
	}

	// start index must be greater than zero and less than vertex count
	//if( startIndex < 0 || startIndex >= vertexCount ) {
	//	return;
	//}

	glDrawArrays(ciriToGlTopology(topology), startIndex, vertexCount);
}

void GLGraphicsDevice::drawIndexed( PrimitiveTopology topology, int indexCount ) {
	if( !_isValid ) {
		return;
	}

	// cannot draw with no active shader
	if( _activeShader.expired() ) {
		return;
	}

	// cannot draw without a valid vertex and index buffer
	if( _activeVertexBuffer.expired() || _activeIndexBuffer.expired() ) {
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
		const std::shared_ptr<GLTexture2D>& texture = std::static_pointer_cast<GLTexture2D>(renderTargets[i]->getTexture2D());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture->getTextureId(), 0);
	}

	// configure draw buffers
	glDrawBuffers(numRenderTargets, _drawBuffers);

	// set viewport (use 0's size)
	setViewport(Viewport(0, 0, renderTargets[0]->getTexture2D()->getWidth(), renderTargets[0]->getTexture2D()->getHeight()));
}

void GLGraphicsDevice::restoreDefaultRenderTargets() {
	if( !_isValid ) {
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	setViewport(Viewport(0, 0, _defaultWidth, _defaultHeight));
}

ErrorCode GLGraphicsDevice::resize() {
	if( !_isValid ) {
		return ErrorCode::CIRI_UNKNOWN_ERROR;
	}

	// todo: check for erroneous sizes

	// get new window size
	const int width = _window->getWidth();
	const int height = _window->getHeight();

	// don't resize if the same size
	if( width == _defaultWidth && height == _defaultHeight ) {
		return ErrorCode::CIRI_OK; // not incorrect; just ignore it
	}

	// update default width and height
	_defaultWidth = width;
	_defaultHeight = height;
		
	// bind backbuffer and resize the viewport
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	setViewport(Viewport(0, 0, _defaultWidth, _defaultHeight));

	return ErrorCode::CIRI_OK;
}

ErrorCode GLGraphicsDevice::resizeTexture2D( const std::shared_ptr<ITexture2D>& texture, int width, int height ) {
	if( !_isValid ) {
		return ErrorCode::CIRI_UNKNOWN_ERROR;
	}

	if( width <= 0 || height <= 0 || nullptr == texture ) {
		return ErrorCode::CIRI_INVALID_ARGUMENT;
	}

	const std::shared_ptr<GLTexture2D> glTexture = std::static_pointer_cast<GLTexture2D>(texture);

	// check for texture that isn't created
	if( 0 == glTexture->getTextureId() ) {
		return ErrorCode::CIRI_UNKNOWN_ERROR;
	}

	glTexture->destroy();
	return glTexture->setData(0, 0, width, height, nullptr, glTexture->getFormat());
}

ErrorCode GLGraphicsDevice::resizeRenderTarget2D( const std::shared_ptr<IRenderTarget2D>& target, int width, int height ) {
	if( !_isValid ) {
		return ErrorCode::CIRI_UNKNOWN_ERROR;
	}

	if( width <= 0 || height <= 0 || nullptr == target ) {
		return ErrorCode::CIRI_INVALID_ARGUMENT;
	}

	const std::shared_ptr<GLRenderTarget2D> glTarget = std::static_pointer_cast<GLRenderTarget2D>(target);

	// check for texture that isn't created
	if( nullptr == glTarget->getTexture2D() ) {
		return ErrorCode::CIRI_UNKNOWN_ERROR;
	}

	// store texture's format for re-creation
	const TextureFormat::Format textureFormat = glTarget->getTexture2D()->getFormat();

	glTarget->destroy();
	const std::shared_ptr<GLTexture2D> glTexture = std::static_pointer_cast<GLTexture2D>(createTexture2D(width, height, textureFormat, TextureFlags::RenderTarget, nullptr));
	if( nullptr == glTexture ) {
		return ErrorCode::CIRI_UNKNOWN_ERROR;
	}
	return glTarget->create(glTexture) ? ErrorCode::CIRI_OK : ErrorCode::CIRI_UNKNOWN_ERROR;
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

void GLGraphicsDevice::setRasterizerState( const std::shared_ptr<IRasterizerState>& state ) {
	if( !_isValid ) {
		return;
	}

	if( nullptr == state ) {
		restoreDefaultRasterizerState();
		return;
	}

	const std::shared_ptr<GLRasterizerState> glRaster = std::static_pointer_cast<GLRasterizerState>(state);
	if( !_activeRasterizerState.expired() && (glRaster == _activeRasterizerState.lock()) ) {
		return;
	}
	_activeRasterizerState = glRaster;
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
	if( desc.depthClipEnable ) { // inverted??
		glEnable(GL_DEPTH_CLAMP);
	} else {
		glDisable(GL_DEPTH_CLAMP);
	}
	// todo: msaa
}

void GLGraphicsDevice::setDepthStencilState( const std::shared_ptr<IDepthStencilState>& state ) {
	if( !_isValid ) {
		return;
	}

	if( nullptr == state ) {
		restoreDefaultDepthStencilState();
		return;
	}

	const std::shared_ptr<GLDepthStencilState> glState = std::static_pointer_cast<GLDepthStencilState>(state);
	if( !_activeDepthStencilState.expired() && (glState == _activeDepthStencilState.lock()) ) {
		return;
	}
	_activeDepthStencilState = glState;
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

std::shared_ptr<IWindow> GLGraphicsDevice::getWindow() const {
	return _window;
}

const char* GLGraphicsDevice::getGpuName() const {
	return _gpuName.c_str();
}

const char* GLGraphicsDevice::getApiInfo() const {
	return _apiInfo.c_str();
}

GraphicsApiType GLGraphicsDevice::getApiType() const {
	return GraphicsApiType::OpenGL;
}

ErrorCode GLGraphicsDevice::restoreDefaultStates() {
	// restore blend state
	ErrorCode status = restoreDefaultBlendState();
	if( failed(status) ) {
		return status;
	}
	// restore rasterizer state
	status = restoreDefaultRasterizerState();
	if( failed(status) ) {
		return status;
	}
	// restore depth stencil state
	status = restoreDefaultDepthStencilState();
	if( failed(status) ) {
		return status;
	}
	return ErrorCode::CIRI_OK;
}

ErrorCode GLGraphicsDevice::restoreDefaultBlendState() {
	setBlendState(getDefaultBlendOpaque());
	return ErrorCode::CIRI_OK;
}

ErrorCode GLGraphicsDevice::restoreDefaultRasterizerState() {
	setRasterizerState(getDefaultRasterCounterClockwise());
	return ErrorCode::CIRI_OK;
}

ErrorCode GLGraphicsDevice::restoreDefaultDepthStencilState() {
	setDepthStencilState(getDefaultDepthStencilDefault());
	return ErrorCode::CIRI_OK;
}

std::shared_ptr<IBlendState> GLGraphicsDevice::getDefaultBlendAdditive() {
	if( nullptr == _defaultBlendAdditive ) {
		BlendDesc desc;
		desc.srcColorBlend = BlendMode::SourceAlpha;
		desc.srcAlphaBlend = BlendMode::SourceAlpha;
		desc.dstColorBlend = BlendMode::One;
		desc.dstAlphaBlend = BlendMode::One;
		_defaultBlendAdditive = createBlendState(desc);
	}
	return _defaultBlendAdditive;
}

std::shared_ptr<IBlendState> GLGraphicsDevice::getDefaultBlendAlpha() {
	if( nullptr == _defaultBlendAlpha ) {
		BlendDesc desc;
		desc.srcColorBlend = BlendMode::One;
		desc.srcAlphaBlend = BlendMode::One;
		desc.dstColorBlend = BlendMode::InverseSourceAlpha;
		desc.dstAlphaBlend = BlendMode::InverseSourceAlpha;
		_defaultBlendAlpha = createBlendState(desc);
	}
	return _defaultBlendAlpha;
}

std::shared_ptr<IBlendState> GLGraphicsDevice::getDefaultBlendNonPremul() {
	if( nullptr == _defaultBlendNonPremul ) {
		BlendDesc desc;
		desc.srcColorBlend = BlendMode::SourceAlpha;
		desc.srcAlphaBlend = BlendMode::SourceAlpha;
		desc.dstColorBlend = BlendMode::InverseSourceAlpha;
		desc.dstAlphaBlend = BlendMode::InverseSourceAlpha;
		_defaultBlendNonPremul = createBlendState(desc);
	}
	return _defaultBlendNonPremul;
}

std::shared_ptr<IBlendState> GLGraphicsDevice::getDefaultBlendOpaque() {
	if( nullptr == _defaultBlendOpaque ) {
		BlendDesc desc;
		desc.srcColorBlend = BlendMode::One;
		desc.srcAlphaBlend = BlendMode::One;
		desc.dstColorBlend = BlendMode::Zero;
		desc.dstAlphaBlend = BlendMode::Zero;
		_defaultBlendOpaque = createBlendState(desc);
	}
	return _defaultBlendOpaque;
}

std::shared_ptr<IRasterizerState> GLGraphicsDevice::getDefaultRasterNone() {
	if( nullptr == _defaultRasterNone ) {
		RasterizerDesc desc;
		desc.cullMode = CullMode::None;
		_defaultRasterNone = createRasterizerState(desc);
	}
	return _defaultRasterNone;
}

std::shared_ptr<IRasterizerState> GLGraphicsDevice::getDefaultRasterClockwise() {
	if( nullptr == _defaultRasterClockwise ) {
		RasterizerDesc desc;
		desc.cullMode = CullMode::Clockwise;
		_defaultRasterClockwise = createRasterizerState(desc);
	}
	return _defaultRasterClockwise;
}

std::shared_ptr<IRasterizerState> GLGraphicsDevice::getDefaultRasterCounterClockwise() {
	if( nullptr == _defaultRasterCounterClockwise ){
		RasterizerDesc desc;
		desc.cullMode = CullMode::CounterClockwise;
		_defaultRasterCounterClockwise = createRasterizerState(desc);
	}
	return _defaultRasterCounterClockwise;
}

std::shared_ptr<IDepthStencilState> GLGraphicsDevice::getDefaultDepthStencilDefault() {
	if( nullptr == _defaultDepthStencilDefault ) {
		DepthStencilDesc desc;
		desc.depthEnable = true;
		desc.depthWriteMask = true;
		_defaultDepthStencilDefault = createDepthStencilState(desc);
	}
	return _defaultDepthStencilDefault;
}

std::shared_ptr<IDepthStencilState> GLGraphicsDevice::getDefaultDepthStencilDepthRead() {
	if( nullptr == _defaultDepthStencilDepthRead ) {
		DepthStencilDesc desc;
		desc.depthEnable = true;
		desc.depthWriteMask = false;
		_defaultDepthStencilDepthRead = createDepthStencilState(desc);
	}
	return _defaultDepthStencilDepthRead;
}

std::shared_ptr<IDepthStencilState> GLGraphicsDevice::getDefaultDepthStencilNone() {
	if( nullptr == _defaultDepthStencilNone ) {
		DepthStencilDesc desc;
		desc.depthEnable = false;
		desc.depthWriteMask = false;
		_defaultDepthStencilNone = createDepthStencilState(desc);
	}
	return _defaultDepthStencilNone;
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
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4, // need 4.2 for shader uniform location binding
		WGL_CONTEXT_MINOR_VERSION_ARB, 2,
		WGL_CONTEXT_FLAGS_ARB, contextFlags,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		//WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB, // not using VAOs; core requires?
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

	// default clear color
	glClearColor(0.39f, 0.58f, 0.93f, 1.0f);

	// *cough*
	// gimmie dat dank core profile (temp hack, unless it works)
	// *cough*
	glGenVertexArrays(1, &_dummyVao);
	glBindVertexArray(_dummyVao);

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
	if( GL_DEBUG_SEVERITY_NOTIFICATION == severity ) {
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

//void GLGraphicsDevice::restoreDefaultBlendState() {
//	if( !_isValid ) {
//		return;
//	}

//	// disable blending
//	glDisable(GL_BLEND);
//	// blend color (http://docs.gl/gl4/glBlendColor)
//	glBlendColor(0.0f, 0.0f, 0.0f, 0.0f);
//	// blend equations (http://docs.gl/gl4/glBlendEquationSeparate)
//	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
//	// blend functions (http://docs.gl/gl4/glBlendFuncSeparate)
//	glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ONE, GL_ZERO);
//	// color write mask (http://docs.gl/gl4/glColorMask)
//	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
//}

namespace ciri { namespace graphics {
std::shared_ptr<IGraphicsDevice> createGraphicsDevice() {
	return std::shared_ptr<IGraphicsDevice>(new GLGraphicsDevice());
}
}}