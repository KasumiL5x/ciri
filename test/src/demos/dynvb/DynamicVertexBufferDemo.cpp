#include "DynamicVertexBufferDemo.hpp"
#include <ciri/core/Log.hpp>
#include <cc/MatrixFunc.hpp>

DynamicVertexBufferDemo::DynamicVertexBufferDemo()
	: IDemo(), _depthStencilState(nullptr), _rasterizerState(nullptr),
		_clothRunning(true) {
}

DynamicVertexBufferDemo::~DynamicVertexBufferDemo() {
}

DemoConfig DynamicVertexBufferDemo::getConfig() {
	DemoConfig cfg;
	cfg.windowTitle = "ciri : Dynamic Vertex Buffer Demo";
	cfg.windowWidth = 1280;
	cfg.windowHeight = 720;
	return cfg;
}

void DynamicVertexBufferDemo::onInitialize() {
	// print driver information
	printf("Device: %s\n", graphicsDevice()->getGpuName());
	printf("API: %s\n", graphicsDevice()->getApiInfo());

	// window size
	const cc::Vec2ui windowSize = window()->getSize();

	// configure the camera
	_camera.setAspect((float)windowSize.x / (float)windowSize.y);
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
	ciri::IGraphicsDevice* device = graphicsDevice();

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

void DynamicVertexBufferDemo::onEvent( ciri::WindowEvent evt ) {
	switch( evt.type) {
		case ciri::WindowEvent::Resized: {
			if( graphicsDevice()->resize() != ciri::err::ErrorCode::CIRI_OK ) {
				printf("Failed to resize default render targets.\n");
			}
			break;
		}
	}
}

void DynamicVertexBufferDemo::onUpdate( double deltaTime, double elapsedTime ) {
	// get current input states
	ciri::KeyboardState currKeyState;
	ciri::MouseState currMouseState;
	ciri::Input::getKeyboardState(&currKeyState);
	ciri::Input::getMouseState(&currMouseState, window());

	// check for close w/ escape
	if( currKeyState.isKeyDown(ciri::Keyboard::Escape) ) {
		this->gtfo();
		return;
	}

	// debug camera info
	if( currKeyState.isKeyDown(ciri::Keyboard::F9) && _prevKeyState.isKeyUp(ciri::Keyboard::F9) ) {
		const cc::Vec3f& pos = _camera.getPosition();
		const float yaw = _camera.getYaw();
		const float pitch = _camera.getPitch();
		const float dolly = _camera.getOffset();
		const cc::Vec3f& target = _camera.getTarget();
		printf("pos(%f/%f/%f); yaw(%f); pitch(%f); dolly(%f); target(%f/%f/%f)\n", pos.x, pos.y, pos.z, yaw, pitch, dolly, target.x, target.y, target.z);
	}

	// camera movement
	if( currKeyState.isKeyDown(ciri::Keyboard::LAlt) ) {
		// rotation
		if( currMouseState.isButtonDown(ciri::MouseButton::Left) ) {
			const float dx = (float)currMouseState.x - (float)_prevMouseState.x;
			const float dy = (float)currMouseState.y - (float)_prevMouseState.y;
			_camera.rotateYaw(-dx);
			_camera.rotatePitch(-dy);
		}
		// dolly
		if( currMouseState.isButtonDown(ciri::MouseButton::Right) ) {
			const float dx = (float)currMouseState.x - (float)_prevMouseState.x;
			const float dy = (float)currMouseState.y - (float)_prevMouseState.y;
			const float val = (fabsf(dx) > fabsf(dy)) ? dx : dy;
			_camera.dolly(val);
		}
		// pan
		if( currMouseState.isButtonDown(ciri::MouseButton::Middle) ) {
			const float dx = (float)currMouseState.x - (float)_prevMouseState.x;
			const float dy = (float)currMouseState.y - (float)_prevMouseState.y;
			_camera.pan(dx, -dy);
		}
	}
	_camera.update(deltaTime);

	// update cloth
	if( currKeyState.isKeyDown(ciri::Keyboard::P) && _prevKeyState.isKeyUp(ciri::Keyboard::P) ) {
		_clothRunning = !_clothRunning;
	}
	if( _clothRunning ) {
		_cloth.update(deltaTime);
	}

	// update previous input states
	_prevKeyState = currKeyState;
	_prevMouseState = currMouseState;
}

void DynamicVertexBufferDemo::onDraw() {
	ciri::IGraphicsDevice* device = graphicsDevice();

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
	_cloth.clean();
	_grid.clean();
}