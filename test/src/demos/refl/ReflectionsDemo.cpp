#include "ReflectionsDemo.hpp"
#include "../../common/ModelGen.hpp"
#include <ciri/util/TGA.hpp>

ReflectionsDemo::ReflectionsDemo()
	: IDemo(), _depthStencilState(nullptr), _rasterizerState(nullptr), _waterPlane(nullptr) {
}

ReflectionsDemo::~ReflectionsDemo() {
}

DemoConfig ReflectionsDemo::getConfig() {
	DemoConfig cfg;
	cfg.windowTitle = "ciri : Reflections Demo";
	cfg.windowWidth = 1280;
	cfg.windowHeight = 720;
	cfg.deviceType = ciri::GraphicsDeviceFactory::OpenGL;
	return cfg;
}

void ReflectionsDemo::onInitialize() {
	// window size
	const cc::Vec2ui windowSize = window()->getSize();

	// configure camera
	_camera.setAspect(static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y));
	_camera.setPlanes(0.1f, 10000.0f);
	_camera.setYaw(238.0f);
	_camera.setPitch(16.4f);
	_camera.setOffset(33.5f);
	_camera.setSensitivity(1.0f, 1.0f, 0.25f);
	_camera.setLerpStrength(100.0f);
	_camera.setTarget(cc::Vec3f(0.0f, 0.0f, 0.0f));
	_camera.resetPosition();
	// debug
	_camera.setYaw(105.0f);
	_camera.setPitch(29.4f);
	_camera.setOffset(422.5f);
	_camera.setTarget(cc::Vec3f(318.17f, -104.05f, -449.94f));
	_camera.resetPosition();

	
}

void ReflectionsDemo::onLoadContent() {
	// create depth stencil state
	ciri::DepthStencilDesc depthDesc;
	_depthStencilState = graphicsDevice()->createDepthStencilState(depthDesc);

	// cerate rasterizer state
	ciri::RasterizerDesc rasterDesc;
	rasterDesc.cullMode = ciri::CullMode::Clockwise;
	//rasterDesc.fillMode = ciri::FillMode::Wireframe;
	_rasterizerState = graphicsDevice()->createRasterizerState(rasterDesc);

	// load the simple shader
	_simpleShader.create(graphicsDevice());

	// create axis
	_axis.create(5.0f, graphicsDevice());

	// create heightmap terrain
	ciri::TGA heightmap;
	heightmap.loadFromFile("refl/heightmap.tga", false);
	if( !_terrain.generate(heightmap, graphicsDevice()) ) {
		printf("Failed to generate heightmap terrain.\n");
	}

	// create water plane to match the heightmap size
	_waterPlane = modelgen::createPlane(heightmap.getWidth(), heightmap.getHeight(), 0, 0, graphicsDevice(), false, false);
	_waterPlane->setShader(_simpleShader.getShader());
	// position in the center of the heightmap
	const float WATER_HEIGHT = 10.0f;
	_waterPlane->getXform().setPosition(cc::Vec3f(heightmap.getWidth()*0.5f, WATER_HEIGHT, -heightmap.getHeight()*0.5f));
}

void ReflectionsDemo::onEvent( ciri::WindowEvent evt ) {
	switch( evt.type ) {
		case ciri::WindowEvent::Resized: {
			graphicsDevice()->resize();
			break;
		}
	}
}

void ReflectionsDemo::onUpdate( double deltaTime, double elapsedTime ) {
	// get current input states
	ciri::KeyboardState currKeyState;
	ciri::MouseState currMouseState;
	ciri::Input::getKeyboardState(&currKeyState);
	ciri::Input::getMouseState(&currMouseState, window());

	// close w/ escape
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

	// camera controls
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

	//// todo: update simulation things here
	//std::vector<Vertex>& planeVertices = _plane->getVertices();
	//for( unsigned int i = 0; i < planeVertices.size(); ++i ) {
	//	const float x = sinf(elapsedTime + planeVertices[i].position.x) * 0.5f;
	//	const float z = sinf(elapsedTime + planeVertices[i].position.z) * 0.5f;
	//	planeVertices[i].position.y = x * z;
	//}
	//_plane->updateBuffers(true, false);

	// update previous input states
	_prevKeyState = currKeyState;
	_prevMouseState = currMouseState;
}

void ReflectionsDemo::onDraw() {
	ciri::IGraphicsDevice* device = graphicsDevice();
	
	device->setDepthStencilState(_depthStencilState);
	device->setRasterizerState(_rasterizerState);

	device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);

	const cc::Mat4f viewProj = _camera.getProj() * _camera.getView();

	// render axis
	if( _axis.isValid() ) {
		_axis.updateConstants(viewProj);
		device->applyShader(_axis.getShader());
		device->setVertexBuffer(_axis.getVertexBuffer());
		device->drawArrays(ciri::PrimitiveTopology::LineList, _axis.getVertexBuffer()->getVertexCount(), 0);
	}

	// render terrain
	if( _terrain.getVertexBuffer() != nullptr && _terrain.getIndexBuffer() != nullptr ) {
		// update shader constants
		_simpleShader.getConstants().world = cc::Mat4f(1.0f);
		_simpleShader.getConstants().xform = viewProj * _simpleShader.getConstants().world;
		_simpleShader.getMaterialConstants().hasDiffuseTexture = 0;
		_simpleShader.getMaterialConstants().diffuseColor = cc::Vec3f(1.0f, 1.0f, 1.0f);
		_simpleShader.updateConstants();

		// apply shader
		device->applyShader(_simpleShader.getShader());

		// set buffers and draw
		device->setVertexBuffer(_terrain.getVertexBuffer());
		device->setIndexBuffer(_terrain.getIndexBuffer());
		device->drawIndexed(ciri::PrimitiveTopology::TriangleList, _terrain.getIndexBuffer()->getIndexCount());
	}

	// render water plane
	if( _waterPlane && _waterPlane->getShader() != nullptr && _waterPlane->isValid() ) {
		// update constant buffers
		_simpleShader.getConstants().world = _waterPlane->getXform().getWorld();
		_simpleShader.getConstants().xform = viewProj * _simpleShader.getConstants().world;
		_simpleShader.getMaterialConstants().hasDiffuseTexture = 0;
		_simpleShader.getMaterialConstants().diffuseColor = cc::Vec3f(0.0f, 0.0f, 1.0f);
		_simpleShader.updateConstants();

		// apply shader
		device->applyShader(_waterPlane->getShader());

		// set buffers and draw
		device->setVertexBuffer(_waterPlane->getVertexBuffer());
		device->setIndexBuffer(_waterPlane->getIndexBuffer());
		device->drawIndexed(ciri::PrimitiveTopology::TriangleList, _waterPlane->getIndexBuffer()->getIndexCount());
	}

	//// render plane
	//if( _plane && _plane->getShader() != nullptr && _plane->getVertexBuffer() != nullptr ) {
	//	// update constant buffers
	//	_simpleShader.getConstants().world = _plane->getXform().getWorld();
	//	_simpleShader.getConstants().xform = viewProj * _simpleShader.getConstants().world;
	//	_simpleShader.getMaterialConstants().hasDiffuseTexture = 0;
	//	_simpleShader.getMaterialConstants().diffuseColor = cc::Vec3f(1.0f, 1.0f, 1.0f);
	//	_simpleShader.updateConstants();
	//	// apply shader
	//	device->applyShader(_plane->getShader());
	//	// set buffers and draw
	//	device->setVertexBuffer(_plane->getVertexBuffer());
	//	if( _plane->getIndexBuffer() != nullptr ) {
	//		device->setIndexBuffer(_plane->getIndexBuffer());
	//		device->drawIndexed(ciri::PrimitiveTopology::TriangleList, _plane->getIndexBuffer()->getIndexCount());
	//	} else {
	//		device->drawArrays(ciri::PrimitiveTopology::TriangleList, _plane->getVertexBuffer()->getVertexCount(), 0);
	//	}
	//}

	// todo: render stuff here

	device->present();
}

void ReflectionsDemo::onUnloadContent() {
	//if( _plane != nullptr ) {
	//	delete _plane;
	//	_plane = nullptr;
	//}
}
