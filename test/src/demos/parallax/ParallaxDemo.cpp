#include "ParallaxDemo.hpp"

ParallaxDemo::ParallaxDemo()
	: Game(), _depthStencilState(nullptr), _rasterizerState(nullptr) {
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

	// create grid
	if( !_grid.create(graphicsDevice()) ) {
		printf("Failed to create grid.\n");
	}

	// create axis
	if( !_axis.create(5.0f, graphicsDevice()) ) {
		printf("Failed to create axis.\n");
	}
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

	// present backbuffer to screen
	device->present();
}

void ParallaxDemo::onUnloadContent() {
	Game::onUnloadContent();

	// clean grid
	_grid.clean();

	// clean axis
	_axis.clean();
}

bool ParallaxDemo::loadShaders() {
	throw;
}

void ParallaxDemo::unloadShaders() {
	throw;
}