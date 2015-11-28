#include "DynamicVertexBufferDemo.hpp"
#include <ciri/core/Log.hpp>
#include <cc/MatrixFunc.hpp>

DynamicVertexBufferDemo::DynamicVertexBufferDemo()
	: Game(), _depthStencilState(nullptr), _rasterizerState(nullptr),
		_clothRunning(true) {
	_config.width = 1280;
	_config.height = 720;
	_config.title = "ciri : Dynamic Vertex Buffer Demo";
}

DynamicVertexBufferDemo::~DynamicVertexBufferDemo() {
}

void* DynamicVertexBufferDemo::operator new( size_t i ) {
	return _mm_malloc(i, 16); // allocate aligned to 16
}

void DynamicVertexBufferDemo::operator delete( void* p ) {
	_mm_free(p);
}

void DynamicVertexBufferDemo::onInitialize() {
	Game::onInitialize();

	// print driver information
	printf("Device: %s\n", graphicsDevice()->getGpuName());
	printf("API: %s\n", graphicsDevice()->getApiInfo());

	// configure the camera
	_camera.setAspect((float)window()->getWidth() / (float)window()->getHeight());
	_camera.setPlanes(0.1f, 1000.0f);
	_camera.setYaw(238.9f);
	_camera.setPitch(16.4f);
	_camera.setOffset(33.5f);
	_camera.setSensitivity(1.0f, 1.0f, 0.25f);
	_camera.setLerpStrength(100.0f);
	_camera.setTarget(cc::Vec3f(0.0f, 8.0f, 5.0f));
	_camera.resetPosition();
}

void DynamicVertexBufferDemo::onLoadContent() {
	Game::onLoadContent();

	std::shared_ptr<ciri::IGraphicsDevice> device = graphicsDevice();

	ciri::DepthStencilDesc depthDesc;
	_depthStencilState = device->createDepthStencilState(depthDesc);
	if( nullptr == _depthStencilState ) {
		ciri::Logs::get(ciri::Logs::Debug).printError("Failed to create depth stencil state.");
	}

	ciri::RasterizerDesc rasterDesc;
	rasterDesc.cullMode = ciri::CullMode::None;//Clockwise;
	//rasterDesc.fillMode = ciri::FillMode::Wireframe;
	_rasterizerState = device->createRasterizerState(rasterDesc);
	if( nullptr == _rasterizerState ) {
		ciri::Logs::get(ciri::Logs::Debug).printError("Failed to create rasterizer state.");
	}

	if( !_grid.create(device) ) {
		ciri::Logs::get(ciri::Logs::Debug).printError("Failed to create grid.");
	}

	if( !_axis.create(5.0f, device) ) {
		ciri::Logs::get(ciri::Logs::Debug).printError("Failed to create axis.");
	}

	if( !_simpleShader.create(device) ) {
		ciri::Logs::get(ciri::Logs::Debug).printError("Failed to create simple shader.");
	}

	if( _flagpole.addFromObj("dynvb/flag-pole.obj") ) {
		if( !_flagpole.build(device) ) {
			ciri::Logs::get(ciri::Logs::Debug).printError("Failed to build flagpole model.obj.");
		}
	} else {
		ciri::Logs::get(ciri::Logs::Debug).printError("Failed to load flag-pole.obj.");
	}
	_flagpole.setShader(_simpleShader.getShader());

	_cloth.setDivisions(20, 20);
	_cloth.setSize(7);
	_cloth.setMass(1.0f);
	_cloth.setDamping(-0.0125f);
	_cloth.setGravity(cc::Vec3f(0.0f, -9.81f, 0.0f));
	_cloth.build(device);
}

void DynamicVertexBufferDemo::onEvent( const ciri::WindowEvent& evt ) {
	Game::onEvent(evt);

	switch( evt.type) {
		case ciri::WindowEvent::Resized: {
			if( graphicsDevice()->resize() != ciri::ErrorCode::CIRI_OK ) {
				printf("Failed to resize default render targets.\n");
			}
			break;
		}
	}
}

void DynamicVertexBufferDemo::onUpdate( const double deltaTime, const double elapsedTime ) {
	Game::onUpdate(deltaTime, elapsedTime);

	// check for close w/ escape
	if( input()->isKeyDown(ciri::Key::Escape) ) {
		this->gtfo();
		return;
	}

	// debug camera info
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
	
	if( input()->isKeyDown(ciri::Key::P) && input()->wasKeyUp(ciri::Key::P) ) {
		_clothRunning = !_clothRunning;
	}

}

void DynamicVertexBufferDemo::onFixedUpdate( const double deltaTime, const double elapsedTime ) {
	Game::onFixedUpdate(deltaTime, elapsedTime);

	_camera.update(static_cast<float>(deltaTime));

	// update cloth
	if( _clothRunning ) {
		_cloth.update(static_cast<float>(deltaTime));
	}
}

void DynamicVertexBufferDemo::onDraw() {
	Game::onDraw();

	std::shared_ptr<ciri::IGraphicsDevice> device = graphicsDevice();

	device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);
	device->setDepthStencilState(_depthStencilState);
	device->setRasterizerState(_rasterizerState);

	// camera's viewproj matrix
	const cc::Mat4f cameraViewProj = _camera.getProj() * _camera.getView();

	// render the grid
	if( _grid.isValid() ) {
		const cc::Mat4f gridXform = cameraViewProj * cc::Mat4f(1.0f);
		if( _grid.updateConstants(gridXform) ) {
			device->applyShader(_grid.getShader());
			device->setVertexBuffer(_grid.getVertexBuffer());
			device->drawArrays(ciri::PrimitiveTopology::LineList, _grid.getVertexBuffer()->getVertexCount(), 0);
		}
	}

	// render the axis
	if( _axis.isValid() ) {
		const cc::Mat4f axisXform = cameraViewProj * cc::Mat4f(1.0f);
		if( _axis.updateConstants(axisXform) ) {
			device->applyShader(_axis.getShader());
			device->setVertexBuffer(_axis.getVertexBuffer());
			device->drawArrays(ciri::PrimitiveTopology::LineList, _axis.getVertexBuffer()->getVertexCount(), 0);
		}
	}

	// render the flagpole
	if( _flagpole.getShader() != nullptr && _flagpole.getShader()->isValid() ) {
		// update constant buffer
		_simpleShader.getConstants().world = _flagpole.getXform().getWorld();
		_simpleShader.getConstants().xform = cameraViewProj * _simpleShader.getConstants().world;
		_simpleShader.getMaterialConstants().hasDiffuseTexture = 0;
		_simpleShader.getMaterialConstants().diffuseColor = cc::Vec3f(1.0f, 1.0f, 1.0f);
		if( !_simpleShader.updateConstants() ) {
			ciri::Logs::get(ciri::Logs::Debug).printError("Failed to update simple constant buffer data.");
		}
		// apply shader
		device->applyShader(_flagpole.getShader());
		// set vertex buffer and index buffer
		device->setVertexBuffer(_flagpole.getVertexBuffer());
		if( _flagpole.getIndexBuffer() != nullptr ) {
			device->setIndexBuffer(_flagpole.getIndexBuffer());
			device->drawIndexed(ciri::PrimitiveTopology::TriangleList, _flagpole.getIndexBuffer()->getIndexCount());
		} else {
			device->drawArrays(ciri::PrimitiveTopology::TriangleList, _flagpole.getVertexBuffer()->getVertexCount(), 0);
		}
	}

	// render the cloth
	if( _cloth.isBuilt() ) {
		device->applyShader(_cloth.getShader());
		_cloth.getConstants().world = cc::math::translate(cc::Vec3f(0, 18.25f, 4)) * cc::math::rotate(90.0f, cc::Vec3f::up());
		_cloth.getConstants().xform = cameraViewProj * _cloth.getConstants().world;
		_cloth.getConstants().camdir = _camera.getPosition();
		_cloth.updateConstants();

		device->setVertexBuffer(_cloth.getVertexBuffer());
		device->setIndexBuffer(_cloth.getIndexBuffer());
		device->drawIndexed(ciri::PrimitiveTopology::TriangleList, _cloth.getIndexBuffer()->getIndexCount());
	}

	device->present();
}

void DynamicVertexBufferDemo::onUnloadContent() {
	Game::onUnloadContent();

	_cloth.clean();
	_grid.clean();
}