#include "DeferredDemo.hpp"

DeferredDemo::DeferredDemo()
	: IDemo() {
}

DeferredDemo::~DeferredDemo() {
}

void* DeferredDemo::operator new( size_t i ) {
	return _mm_malloc(i, 16);
}

void DeferredDemo::operator delete( void* p ) {
	_mm_free(p);
}

DemoConfig DeferredDemo::getConfig() {
	DemoConfig cfg;
	cfg.windowTitle = "ciri : Deferred Demo";
	cfg.windowWidth = 1280;
	cfg.windowHeight = 720;
	return cfg;
}

void DeferredDemo::onInitialize() {
	printf("Device: %s\n", graphicsDevice()->getGpuName());
	printf("API: %s\n", graphicsDevice()->getApiInfo());

	// configure camera
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

void DeferredDemo::onLoadContent() {
}

void DeferredDemo::onEvent( ciri::WindowEvent evt ) {
	switch( evt.type ) {
		case ciri::WindowEvent::Resized: {
			if( graphicsDevice()->resize() != ciri::ErrorCode::CIRI_OK ) {
				printf("Failed to resize default render targets.\n");
			}
			break;
		}
	}
}

void DeferredDemo::onUpdate( double deltaTime, double elapsedTime ) {
	if( input()->isKeyDown(ciri::Key::Escape) ) {
		this->gtfo();
		return;
	}

	if( input()->isKeyDown(ciri::Key::F9) && input()->wasKeyUp(ciri::Key::F9) ) {
		const cc::Vec3f& pos = _camera.getPosition();
		const float yaw = _camera.getYaw();
		const float pitch = _camera.getPitch();
		const float dolly = _camera.getOffset();
		const cc::Vec3f& target = _camera.getTarget();
		printf("pos(%f/%f/%f); yaw(%f); pitch(%f); dolly(%f); target(%f/%f/%f)\n", pos.x, pos.y, pos.z, yaw, pitch, dolly, target.x, target.y, target.z);
	}

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

void DeferredDemo::onFixedUpdate( double deltaTime, double elapsedTime ) {
	_camera.update(static_cast<float>(deltaTime));
}

void DeferredDemo::onDraw() {
	std::shared_ptr<ciri::IGraphicsDevice> device = graphicsDevice();

	device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);
	device->present();
}

void DeferredDemo::onUnloadContent() {
}

