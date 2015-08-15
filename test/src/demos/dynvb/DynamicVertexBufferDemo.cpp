#include "DynamicVertexBufferDemo.hpp"

DynamicVertexBufferDemo::DynamicVertexBufferDemo()
	: IDemo(), _graphicsDevice(nullptr) {
}

DynamicVertexBufferDemo::~DynamicVertexBufferDemo() {
}

DemoConfig DynamicVertexBufferDemo::getConfig() {
	DemoConfig cfg;
	cfg.deviceType = ciri::GraphicsDeviceFactory::OpenGL;
	return cfg;
}

void DynamicVertexBufferDemo::onInitialize( ciri::Window* window, ciri::IGraphicsDevice* graphicsDevice ) {
	//printf("onInitialize()\n");
	_graphicsDevice = graphicsDevice;
}

void DynamicVertexBufferDemo::onLoadContent() {
	//printf("onLoadContent()\n");
}

void DynamicVertexBufferDemo::onEvent( ciri::WindowEvent evt ) {
	//printf("onEvent()\n");
}

void DynamicVertexBufferDemo::onUpdate( double deltaTime, double elapsedTime ) {
	//printf("dt: %f; et: %f\n", deltaTime, elapsedTime);
}

void DynamicVertexBufferDemo::onDraw() {
	_graphicsDevice->clear(ciri::ClearFlags::Color);
	_graphicsDevice->present();
}

void DynamicVertexBufferDemo::onUnloadContent() {
	//printf("onUnloadContent()\n");
}