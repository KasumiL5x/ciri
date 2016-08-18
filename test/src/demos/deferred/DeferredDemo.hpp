#ifndef __deferred_demo__
#define __deferred_demo__

#include <ciri/Game.hpp>

class DeferredDemo : public ciri::App {
public:
	DeferredDemo();
	virtual ~DeferredDemo();

	virtual void onInitialize();
	virtual void onLoadContent();
	virtual void onEvent(const ciri::WindowEvent& evt);
	virtual void onUpdate(const double deltaTime, const double elapsedTime);
	virtual void onFixedUpdate(const double deltaTime, const double elapsedTime);
	virtual void onDraw();
	virtual void onUnloadContent();
};


#endif