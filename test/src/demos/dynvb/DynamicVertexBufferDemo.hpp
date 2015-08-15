#ifndef __dynamicvertexbufferdemo__
#define __dynamicvertexbufferdemo__

#include "../IDemo.hpp"

class DynamicVertexBufferDemo : public IDemo {
public:
	DynamicVertexBufferDemo();
	virtual ~DynamicVertexBufferDemo();

	virtual DemoConfig getConfig();
	virtual void onInitialize( ciri::Window* window, ciri::IGraphicsDevice* graphicsDevice );
	virtual void onLoadContent();
	virtual void onEvent( ciri::WindowEvent evt );
	virtual void onUpdate( double deltaTime, double elapsedTime );
	virtual void onDraw();
	virtual void onUnloadContent();

private:
	ciri::IGraphicsDevice* _graphicsDevice;
};

#endif /* __dynamicvertexbufferdemo__ */