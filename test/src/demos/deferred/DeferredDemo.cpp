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
}

void DeferredDemo::onLoadContent() {
}

void DeferredDemo::onEvent( ciri::WindowEvent evt ) {
}

void DeferredDemo::onUpdate( double deltaTime, double elapsedTime ) {
}

void DeferredDemo::onFixedUpdate( double deltaTime, double elapsedTime ) {
}

void DeferredDemo::onDraw() {
}

void DeferredDemo::onUnloadContent() {
}

