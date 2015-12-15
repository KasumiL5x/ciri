#include "RefractDemo.hpp"

RefractDemo::RefractDemo()
	: Game(), _model(nullptr) {
	_config.width = 1280;
	_config.height = 720;
	_config.title = "ciri : Refraction Demo";
}

RefractDemo::~RefractDemo() {
}

void* RefractDemo::operator new( size_t i ) {
	return _mm_malloc(i, 16);
}

void RefractDemo::operator delete( void* p ) {
	_mm_free(p);
}

void RefractDemo::onInitialize() {
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

void RefractDemo::onLoadContent() {
	Game::onLoadContent();

	// create grid
	if( !_grid.create(graphicsDevice()) ) {
		printf("Failed to create grid.\n");
	}

	// create axis
	if( !_axis.create(5.0f, graphicsDevice()) ) {
		printf("Failed to create axis.\n");
	}

	// load shaders
	if( !loadShaders() ) {
		printf("Failed to load shaders.\n");
	}

	// load model
	_model = modelgen::createSphere(graphicsDevice(), 32, 10.0f);
	if( _model != nullptr ) {
		_model->setShader(_refractShader);
	} else {
		printf("Failed to create model.\n");
	}

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
}

void RefractDemo::onEvent( const ciri::WindowEvent& evt ) {
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

void RefractDemo::onUpdate( const double deltaTime, const double elapsedTime ) {
	Game::onUpdate(deltaTime, elapsedTime);

	// check for close w/ escape
	if( input()->isKeyDown(ciri::Key::Escape) ) {
		gtfo();
		return;
	}

	// debug print camera information
	if( input()->isKeyDown(ciri::Key::F9) && input()->wasKeyUp(ciri::Key::F9) ) {
		const cc::Vec3f& pos = _camera.getPosition();
		const float yaw = _camera.getYaw();
		const float pitch = _camera.getPitch();
		const float dolly = _camera.getOffset();
		const cc::Vec3f& target = _camera.getTarget();
		printf("pos(%f/%f/%f); yaw(%f); pitch(%f); dolly(%f); target(%f/%f/%f)\n", pos.x, pos.y, pos.z, yaw, pitch, dolly, target.x, target.y, target.z);
	}

	// camera movement
	if( input()->isKeyDown(ciri::Key::LAlt) ) {
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

void RefractDemo::onFixedUpdate( const double deltaTime, const double elapsedTime ) {
	Game::onFixedUpdate(deltaTime, elapsedTime);

	// update camera
	_camera.update((float)deltaTime);
}

void RefractDemo::onDraw() {
	Game::onDraw();

	std::shared_ptr<ciri::IGraphicsDevice> device = graphicsDevice();

	// camera's viewproj
	const cc::Mat4f cameraViewProj = _camera.getProj() * _camera.getView();

	// clear backbuffer
	device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);

	// set depth and raster states
	device->setDepthStencilState(_depthStencilState);
	device->setRasterizerState(_rasterizerState);
	
	// render grid
	if( _grid.isValid() ) {
		const cc::Mat4f gridXform = cameraViewProj * cc::Mat4f(1.0f);
		if( _grid.updateConstants(gridXform) ) {
			device->applyShader(_grid.getShader());
			device->setVertexBuffer(_grid.getVertexBuffer());
			device->drawArrays(ciri::PrimitiveTopology::LineList, _grid.getVertexBuffer()->getVertexCount(), 0);
		}
	}

	// render axis
	if( _axis.isValid() ) {
		const cc::Mat4f axisXform = cameraViewProj * cc::Mat4f(1.0f);
		if( _axis.updateConstants(axisXform) ) {
			device->applyShader(_axis.getShader());
			device->setVertexBuffer(_axis.getVertexBuffer());
			device->drawArrays(ciri::PrimitiveTopology::LineList, _axis.getVertexBuffer()->getVertexCount(), 0);
		}
	}

	// render model
	if( _refractShader != nullptr && _model != nullptr && _model->isValid() ) {
		// update constant buffer
		_refractVertexConstants.xform = _model->getXform().getWorld();
		_refractVertexConstants.xform = cameraViewProj * _refractVertexConstants.world;
		if( ciri::failed(_refractVertexConstantBuffer->setData(sizeof(RefractVertexConstants), &_refractVertexConstants)) ) {
			printf("Failed to update constants.\n");
		}
		// apply shader
		device->applyShader(_refractShader);
		// set vertex and index buffer and draw
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

void RefractDemo::onUnloadContent() {
	Game::onUnloadContent();

	if( _model != nullptr ) {
		delete _model;
		_model = nullptr;
	}

	// clean grid
	_grid.clean();

	// clean axis
	_axis.clean();
}

bool RefractDemo::loadShaders() {
	// create shader
	_refractShader = graphicsDevice()->createShader();

	// add input elements to shader
	_refractShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Position, 0));
	_refractShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Normal, 0));
	_refractShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float4, ciri::VertexUsage::Tangent, 0));
	_refractShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float2, ciri::VertexUsage::Texcoord, 0));

	// load shader from file
	const std::string shaderExt = graphicsDevice()->getShaderExt();
	const std::string vsFile = ("refract/refract_vs" + shaderExt);
	const std::string psFile = ("refract/refract_ps" + shaderExt);
	if( ciri::failed(_refractShader->loadFromFile(vsFile.c_str(), nullptr, psFile.c_str())) ) {
		printf("Failed to load shader.\n");
		return false;
	}

	// create constant buffer
	_refractVertexConstantBuffer = graphicsDevice()->createConstantBuffer();
	if( ciri::failed(_refractVertexConstantBuffer->setData(sizeof(RefractVertexConstants), &_refractVertexConstants)) ) {
		printf("Failed to create constant buffer.\n");
		return false;
	}

	// assign constant buffer
	if( ciri::failed(_refractShader->addConstants(_refractVertexConstantBuffer, "RefractVertexConstants", ciri::ShaderStage::Vertex)) ) {
		printf("Failed to assign constant buffer to shader.\n");
		return false;
	}

	return true;
}