#include "ParallaxDemo.hpp"
#include "../../common/ModelGen.hpp"

ParallaxDemo::ParallaxDemo()
	: Game(), _depthStencilState(nullptr), _rasterizerState(nullptr), _model(nullptr), _parallaxShader(nullptr),
		_parallaxVertexConstantBuffer(nullptr), _parallaxTexture(nullptr), _parallaxSampler(nullptr) {
	_config.width = 1280;
	_config.height = 720;
	_config.title = "ciri : Parallax Demo";
}

ParallaxDemo::~ParallaxDemo() {
}

void* ParallaxDemo::operator new( size_t i ) {
	return _mm_malloc(i, 16);
}

void ParallaxDemo::operator delete( void* p ) {
	_mm_free(p);
}

void ParallaxDemo::onInitialize() {
	Game::onInitialize();

	// print driver information
	printf("Device: %s\n", graphicsDevice()->getGpuName());
	printf("API: %s\n", graphicsDevice()->getApiInfo());
	
	// configure camera
	_camera.setAspect((float)window()->getWidth() / (float)window()->getHeight());
	_camera.setPlanes(0.1f, 1000.0f);
	_camera.setYaw(59.0f);
	_camera.setPitch(18.f);
	_camera.setOffset(53.0f);
	_camera.setSensitivity(1.0f, 1.0f, 0.25f);
	_camera.setLerpStrength(100.0f);
	_camera.setTarget(cc::Vec3f(0.0f, 0.0f, 0.0f));
	_camera.resetPosition();
}

void ParallaxDemo::onLoadContent() {
	Game::onLoadContent();

	// create depth stencil state
	ciri::DepthStencilDesc depthDesc;
	_depthStencilState = graphicsDevice()->createDepthStencilState(depthDesc);
	if( nullptr == _depthStencilState ) {
		printf("Failed to create depth stencil state.\n");
	}

	// create rasterizer state
	ciri::RasterizerDesc rasterDesc;
	rasterDesc.cullMode = ciri::CullMode::None;
	//rasterDesc.fillMode = ciri::FillMode::Wireframe;
	_rasterizerState = graphicsDevice()->createRasterizerState(rasterDesc);
	if( nullptr == _rasterizerState ) {
		printf("Failed to create rasterizer state.\n");
	}

	// load shaders
	if( !loadShaders() ) {
		printf("Failed to load shaders.\n");
	}

	// create grid
	if( !_grid.create(graphicsDevice()) ) {
		printf("Failed to create grid.\n");
	}

	// create axis
	if( !_axis.create(5.0f, graphicsDevice()) ) {
		printf("Failed to create axis.\n");
	}

	// load model
	//_model = modelgen::createCube(graphicsDevice(), 10.0f, 10.0f, 10.0f, 1.0f, 1.0f, true);
	_model = modelgen::createPlane(graphicsDevice(), 40.0f, 40.0f, 0, 0, 1.0f, 1.0f, false, false);
	if( nullptr == _model ) {
		printf("Failed to load model.\n");
	}

	// load diffuse texture
	ciri::PNG diffuse;
	diffuse.loadFromFile("parallax/metal.png", true);
	_diffuseTexture = graphicsDevice()->createTexture2D(diffuse.getWidth(), diffuse.getHeight(), ciri::TextureFormat::RGBA32_UINT, 0, diffuse.getPixels());

	// load normal texture
	ciri::PNG normal;
	normal.loadFromFile("parallax/metal_n.png", true);
	_normalTexture = graphicsDevice()->createTexture2D(normal.getWidth(), normal.getHeight(), ciri::TextureFormat::RGBA32_UINT, 0, normal.getPixels());

	// load parallax texture
	ciri::PNG png;
	png.loadFromFile("parallax/bricks.png", true);
	_parallaxTexture = graphicsDevice()->createTexture2D(png.getWidth(), png.getHeight(), ciri::TextureFormat::RGBA32_UINT, 0, png.getPixels());

	// create parallax texture sampler
	ciri::SamplerDesc samplerDesc;
	samplerDesc.wrapU = samplerDesc.wrapV = samplerDesc.wrapW = ciri::SamplerWrap::Wrap;
	samplerDesc.filter = ciri::SamplerFilter::Anisotropic;
	samplerDesc.maxAnisotropy = 16;
	_parallaxSampler = graphicsDevice()->createSamplerState(samplerDesc);
}

void ParallaxDemo::onEvent( const ciri::WindowEvent& evt ) {
	Game::onEvent(evt);

	switch( evt.type ) {
		case ciri::WindowEvent::Resized: {
			if( graphicsDevice()->resize() != ciri::ErrorCode::CIRI_OK ) {
				printf("Failed to resize backbuffer.\n");
			}
			break;
		}
	}
}

void ParallaxDemo::onUpdate( const double deltaTime, const double elapsedTime ) {
	Game::onUpdate(deltaTime, elapsedTime);

	// check for close w/ escape
	if( window()->hasFocus() && input()->isKeyDown(ciri::Key::Escape) ) {
		gtfo();
		return;
	}

	// reload shaders
	if( window()->hasFocus() && input()->isKeyDown(ciri::Key::F5) && input()->wasKeyUp(ciri::Key::F5) ) {
		unloadShaders();
		printf("Reloaded shaders: %s\n", loadShaders() ? "success" : "failed");
	}

	// debug print camera information
	if( window()->hasFocus() && input()->isKeyDown(ciri::Key::F9) && input()->wasKeyUp(ciri::Key::F9) ) {
		const cc::Vec3f& pos = _camera.getPosition();
		const float yaw = _camera.getYaw();
		const float pitch = _camera.getPitch();
		const float dolly = _camera.getOffset();
		const cc::Vec3f& target = _camera.getTarget();
		printf("pos(%f/%f/%f); yaw(%f); pitch(%f); dolly(%f); target(%f/%f/%f)\n", pos.x, pos.y, pos.z, yaw, pitch, dolly, target.x, target.y, target.z);
	}

	// camera movement
	if( window()->hasFocus() && input()->isKeyDown(ciri::Key::LAlt) ) {
		// rotation
		if( input()->isMouseButtonDown(ciri::MouseButton::Left) ) {
			const float dx = (float)input()->mouseX() - (float)input()->lastMouseX();
			const float dy = (float)input()->mouseY() - (float)input()->lastMouseY();
			_camera.rotateYaw(-dx);
			_camera.rotatePitch(-dy);
		}
		// dolly
		if( input()->isMouseButtonDown(ciri::MouseButton::Right) ) {
			const float dx = (float)input()->mouseX() - (float)input()->lastMouseX();
			const float dy = (float)input()->mouseY() - (float)input()->lastMouseY();
			const float val = (fabsf(dx) > fabsf(dy)) ? dx : dy;
			_camera.dolly(val);
		}
		// pan
		if( input()->isMouseButtonDown(ciri::MouseButton::Middle) ) {
			const float dx = (float)input()->mouseX() - (float)input()->lastMouseX();
			const float dy = (float)input()->mouseY() - (float)input()->lastMouseY();
			_camera.pan(dx, -dy);
		}
	}
}

void ParallaxDemo::onFixedUpdate( const double deltaTime, const double elapsedTime ) {
	Game::onFixedUpdate(deltaTime, elapsedTime);

	// update camera
	_camera.update(static_cast<float>(deltaTime));
}

void ParallaxDemo::onDraw() {
	Game::onDraw();

	std::shared_ptr<ciri::IGraphicsDevice> device = graphicsDevice();

	// camera's viewproj
	const cc::Mat4f cameraViewProj = _camera.getProj() * _camera.getView();

	// clear backbuffer
	device->setClearColor(0.16f, 0.16f, 0.16f, 1.0f);
	device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);

	// set depth and raster states
	device->setDepthStencilState(_depthStencilState);
	device->setRasterizerState(_rasterizerState);

	// render grid
	if( false && _grid.isValid() ) {
		const cc::Mat4f gridXform = cameraViewProj * cc::Mat4f(1.0f);
		if( _grid.updateConstants(gridXform) ) {
			device->applyShader(_grid.getShader());
			device->setVertexBuffer(_grid.getVertexBuffer());
			device->drawArrays(ciri::PrimitiveTopology::LineList, _grid.getVertexBuffer()->getVertexCount(), 0);
		}
	}

	// render axis
	if( false && _axis.isValid() ) {
		const cc::Mat4f axisXform = cameraViewProj * cc::Mat4f(1.0f);
		if( _axis.updateConstants(axisXform) ) {
			device->applyShader(_axis.getShader());
			device->setVertexBuffer(_axis.getVertexBuffer());
			device->drawArrays(ciri::PrimitiveTopology::LineList, _axis.getVertexBuffer()->getVertexCount(), 0);
		}
	}

	// render model
	if( _parallaxShader && _parallaxShader->isValid() && _model && _model->isValid() ) {
		// update constant buffer
		_parallaxVertexConstants.world = _model->getXform().getWorld();
		_parallaxVertexConstants.xform = cameraViewProj * _parallaxVertexConstants.world;
		_parallaxVertexConstants.campos = _camera.getPosition();
		if( ciri::failed(_parallaxVertexConstantBuffer->setData(sizeof(ParallaxVertexConstants), &_parallaxVertexConstants)) ) {
			printf("Failed to update constant buffer.\n");
		}

		// apply shader
		device->applyShader(_parallaxShader);

		// set textures and samplers
		device->setTexture2D(0, _diffuseTexture, ciri::ShaderStage::Pixel);
		device->setTexture2D(1, _normalTexture, ciri::ShaderStage::Pixel);
		device->setTexture2D(2, _parallaxTexture, ciri::ShaderStage::Pixel);
		device->setSamplerState(0, _parallaxSampler, ciri::ShaderStage::Pixel);
		device->setSamplerState(1, _parallaxSampler, ciri::ShaderStage::Pixel);
		device->setSamplerState(2, _parallaxSampler, ciri::ShaderStage::Pixel);

		// set buffers and draw
		device->setVertexBuffer(_model->getVertexBuffer());
		if( _model->getIndexBuffer() != nullptr ) {
			device->setIndexBuffer(_model->getIndexBuffer());
			device->drawIndexed(ciri::PrimitiveTopology::TriangleList, _model->getIndexBuffer()->getIndexCount());
		} else {
			device->drawArrays(ciri::PrimitiveTopology::TriangleList, _model->getVertexBuffer()->getVertexCount(), 0);
		}
	}

	// present backbuffer to screen
	device->present();
}

void ParallaxDemo::onUnloadContent() {
	Game::onUnloadContent();

	// clean model
	if( _model != nullptr ) {
		delete _model;
		_model = nullptr;
	}

	// clean grid
	_grid.clean();

	// clean axis
	_axis.clean();
}

bool ParallaxDemo::loadShaders() {
	// create shader
	if( nullptr == _parallaxShader ) {
		_parallaxShader = graphicsDevice()->createShader();
		// add input elements
		_parallaxShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Position, 0));
		_parallaxShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Normal, 0));
		_parallaxShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float4, ciri::VertexUsage::Tangent, 0));
		_parallaxShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float2, ciri::VertexUsage::Texcoord, 0));
	}

	// load shader from file
	const std::string shaderExt = graphicsDevice()->getShaderExt();
	const std::string vsFile = ("parallax/parallax_vs" + shaderExt);
	const std::string psFile = ("parallax/parallax_ps" + shaderExt);
	if( ciri::failed(_parallaxShader->loadFromFile(vsFile.c_str(), nullptr, psFile.c_str())) ) {
		printf("Shader failed to compile with %d errors:\n", _parallaxShader->getErrors().size());
		unsigned int idx = 0;
		for( const auto& err : _parallaxShader->getErrors() ) {
			printf("[%d]: %s\n", idx, err.msg.c_str());
			idx += 1;
		}
		return false;
	}

	// create constant buffer
	if( nullptr == _parallaxVertexConstantBuffer) {
		_parallaxVertexConstantBuffer = graphicsDevice()->createConstantBuffer();
	}
	// set constant buffer data
	if( ciri::failed(_parallaxVertexConstantBuffer->setData(sizeof(ParallaxVertexConstants), &_parallaxVertexConstants)) ) {
		printf("Failed to assign data to constant buffer.\n");
		return false;
	}

	// assign constant buffer to shader
	if( ciri::failed(_parallaxShader->addConstants(_parallaxVertexConstantBuffer, "ParallaxVertexConstants", ciri::ShaderStage::Vertex)) ) {
		printf("Failed to assign constant buffer to shader.\n");
		return false;
	}

	return true;
}

void ParallaxDemo::unloadShaders() {
	// clean constant buffer
	if( _parallaxVertexConstantBuffer != nullptr ) {
		_parallaxVertexConstantBuffer->destroy();
	}

	// clean shader
	if( _parallaxShader != nullptr ) {
		_parallaxShader->destroy();
	}
}