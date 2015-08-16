#include "DynamicVertexBufferDemo.hpp"
#include <ciri/util/Log.hpp>

DynamicVertexBufferDemo::DynamicVertexBufferDemo()
	: IDemo(), _graphicsDevice(nullptr), _window(nullptr) {
}

DynamicVertexBufferDemo::~DynamicVertexBufferDemo() {
}

DemoConfig DynamicVertexBufferDemo::getConfig() {
	DemoConfig cfg;
	cfg.deviceType = ciri::GraphicsDeviceFactory::OpenGL;
	_shaderExtension = (ciri::GraphicsDeviceFactory::OpenGL == cfg.deviceType) ? ".glsl" : ".hlsl";
	return cfg;
}

void DynamicVertexBufferDemo::onInitialize( ciri::Window* window, ciri::IGraphicsDevice* graphicsDevice ) {
	_window = window;
	_graphicsDevice = graphicsDevice;

	// window size
	const cc::Vec2ui windowSize = window->getSize();

	// configure the camera
	_camera.setAspect((float)windowSize.x / (float)windowSize.y);
	_camera.setPlanes(0.1f, 1000.0f);
	_camera.setYaw(247.9f);
	_camera.setPitch(22.4f);
	_camera.setOffset(49.5f);
	_camera.setSensitivity(100.0f, 50.0f, 10.0f);
	_camera.setLerpStrength(100.0f);
	_camera.resetPosition();
}

void DynamicVertexBufferDemo::onLoadContent() {
	if( !_grid.create(_graphicsDevice, _shaderExtension) ) {
		ciri::Logs::get(ciri::Logs::Debug).printError("Failed to create grid.");
	}

	if( !_axis.create(5.0f, _shaderExtension, _graphicsDevice) ) {
		ciri::Logs::get(ciri::Logs::Debug).printError("Failed to create axis.");
	}

	if( !_simpleShader.create(_graphicsDevice, _shaderExtension) ) {
		ciri::Logs::get(ciri::Logs::Debug).printError("Failed to create simple shader.");
	}

	if( _flagpole.addFromObj("dynvb/flag-pole.obj") ) {
		if( !_flagpole.build(_graphicsDevice) ) {
			ciri::Logs::get(ciri::Logs::Debug).printError("Failed to build flagpole model.obj.");
		}
	} else {
		ciri::Logs::get(ciri::Logs::Debug).printError("Failed to load flag-pole.obj.");
	}
	_flagpole.setShader(_simpleShader.getShader());
}

void DynamicVertexBufferDemo::onEvent( ciri::WindowEvent evt ) {
}

void DynamicVertexBufferDemo::onUpdate( double deltaTime, double elapsedTime ) {
	// get current input states
	ciri::KeyboardState currKeyState;
	ciri::MouseState currMouseState;
	ciri::Input::getKeyboardState(&currKeyState);
	ciri::Input::getMouseState(&currMouseState, _window);

	// check for close w/ escape
	if( currKeyState.isKeyDown(ciri::Keyboard::Escape) ) {
		_window->destroy();
		return;
	}

	// debug camera info
	if( currKeyState.isKeyDown(ciri::Keyboard::F9) && _prevKeyState.isKeyUp(ciri::Keyboard::F9) ) {
		const cc::Vec3f& pos = _camera.getPosition();
		const float yaw = _camera.getYaw();
		const float pitch = _camera.getPitch();
		const float dolly = _camera.getOffset();
		printf("pos(%f/%f/%f); yaw(%f); pitch(%f); dolly(%f)\n", pos.x, pos.y, pos.z, yaw, pitch, dolly);
	}

	// camera movement
	if( currKeyState.isKeyDown(ciri::Keyboard::LAlt) ) {
		// rotation
		if( currMouseState.isButtonDown(ciri::MouseButton::Left) ) {
			const float dx = (float)currMouseState.x - (float)_prevMouseState.x;
			const float dy = (float)currMouseState.y - (float)_prevMouseState.y;
			_camera.rotateYaw(-dx * deltaTime);
			_camera.rotatePitch(-dy * deltaTime);
		}
		// dolly
		if( currMouseState.isButtonDown(ciri::MouseButton::Right) ) {
			const float dx = (float)currMouseState.x - (float)_prevMouseState.x;
			const float dy = (float)currMouseState.y - (float)_prevMouseState.y;
			const float val = (fabsf(dx) > fabsf(dy)) ? dx : dy;
			_camera.dolly(val * deltaTime);
		}
		// pan
		if( currMouseState.isButtonDown(ciri::MouseButton::Middle) ) {
			const float dx = (float)currMouseState.x - (float)_prevMouseState.x;
			const float dy = (float)currMouseState.y - (float)_prevMouseState.y;
			_camera.pan(dx * deltaTime, -dy * deltaTime);
		}
	}
	_camera.update(deltaTime);

	// update previous input states
	_prevKeyState = currKeyState;
	_prevMouseState = currMouseState;
}

void DynamicVertexBufferDemo::onDraw() {
	_graphicsDevice->clear(ciri::ClearFlags::Color);

	// camera's viewproj matrix
	const cc::Mat4f cameraViewProj = _camera.getProj() * _camera.getView();

	// render the grid
	if( _grid.isValid() ) {
		const cc::Mat4f gridXform = cameraViewProj * cc::Mat4f(1.0f);
		if( _grid.updateConstants(gridXform) ) {
			_graphicsDevice->applyShader(_grid.getShader());
			_graphicsDevice->setVertexBuffer(_grid.getVertexBuffer());
			_graphicsDevice->drawArrays(ciri::PrimitiveTopology::LineList, _grid.getVertexBuffer()->getVertexCount(), 0);
		}
	}

	// render the axis
	if( _axis.isValid() ) {
		const cc::Mat4f axisXform = cameraViewProj * cc::Mat4f(1.0f);
		if( _axis.updateConstants(axisXform) ) {
			_graphicsDevice->applyShader(_axis.getShader());
			_graphicsDevice->setVertexBuffer(_axis.getVertexBuffer());
			_graphicsDevice->drawArrays(ciri::PrimitiveTopology::LineList, _axis.getVertexBuffer()->getVertexCount(), 0);
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
		_graphicsDevice->applyShader(_flagpole.getShader());
		// set vertex buffer and index buffer
		_graphicsDevice->setVertexBuffer(_flagpole.getVertexBuffer());
		if( _flagpole.getIndexBuffer() != nullptr ) {
			_graphicsDevice->setIndexBuffer(_flagpole.getIndexBuffer());
			_graphicsDevice->drawIndexed(ciri::PrimitiveTopology::TriangleList, _flagpole.getIndexBuffer()->getIndexCount());
		} else {
			_graphicsDevice->drawArrays(ciri::PrimitiveTopology::TriangleList, _flagpole.getVertexBuffer()->getVertexCount(), 0);
		}
	}

	_graphicsDevice->present();
}

void DynamicVertexBufferDemo::onUnloadContent() {
	_grid.clean();
}