#ifndef __reflections_demo__
#define __reflections_demo__

#include "../IDemo.hpp"

class ReflectionsDemo : public IDemo {
public:
	ReflectionsDemo();
	virtual ~ReflectionsDemo();

	virtual DemoConfig getConfig();
	virtual void onInitialize();
	virtual void onLoadContent();
	virtual void onEvent( ciri::WindowEvent evt );
	virtual void onUpdate( double deltaTime, double elapsedTime );
	virtual void onDraw();
	virtual void onUnloadContent();
};

#endif /* __reflections_demo__ */
