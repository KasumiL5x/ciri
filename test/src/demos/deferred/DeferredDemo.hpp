#ifndef __deferreddemo__
#define __deferreddemo__

#include "../IDemo.hpp"

class DeferredDemo : public IDemo {
public:
	DeferredDemo();
	virtual ~DeferredDemo();
	void* operator new( size_t i );
	void operator delete( void* p );

	virtual DemoConfig getConfig() override;
	virtual void onInitialize() override;
	virtual void onLoadContent() override;
	virtual void onEvent( ciri::WindowEvent evt ) override;
	virtual void onUpdate( double deltaTime, double elapsedTime ) override;
	virtual void onFixedUpdate( double deltaTime, double elapsedTime ) override;
	virtual void onDraw() override;
	virtual void onUnloadContent() override;
};

#endif /* __deferreddemo__ */