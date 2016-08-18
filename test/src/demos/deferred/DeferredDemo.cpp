#include "DeferredDemo.hpp"

DeferredDemo::DeferredDemo() {
}

DeferredDemo::~DeferredDemo() {
}

void DeferredDemo::onInitialize() {
	App::onInitialize();
}

void DeferredDemo::onLoadContent() {
	App::onLoadContent();
}

void DeferredDemo::onEvent(const ciri::WindowEvent& evt) {
	App::onEvent(evt);
}

void DeferredDemo::onUpdate(const double deltaTime, const double elapsedTime) {
	App::onUpdate(deltaTime, elapsedTime);
}

void DeferredDemo::onFixedUpdate(const double deltaTime, const double elapsedTime) {
	App::onFixedUpdate(deltaTime, elapsedTime);
}

void DeferredDemo::onDraw() {
	App::onDraw();
}

void DeferredDemo::onUnloadContent() {
	App::onUnloadContent();
}