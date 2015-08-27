#include "ReflectionsDemo.hpp"

ReflectionsDemo::ReflectionsDemo()
	: IDemo() {
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
}

void ReflectionsDemo::onLoadContent() {
}

void ReflectionsDemo::onEvent( ciri::WindowEvent evt ) {
}

void ReflectionsDemo::onUpdate( double deltaTime, double elapsedTime ) {
}

void ReflectionsDemo::onDraw() {
}

void ReflectionsDemo::onUnloadContent() {
}
