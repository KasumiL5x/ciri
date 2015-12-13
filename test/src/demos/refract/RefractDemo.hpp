#ifndef __refractdemo__
#define __refractdemo__

#include "../../framework/Game.hpp"
#include <ciri/gfx/MayaCamera.hpp>
#include "../../common/Grid.hpp"
#include "../../common/Axis.hpp"

class RefractDemo : public Game {
public:
	RefractDemo();
	virtual ~RefractDemo();
	void* operator new( size_t i );
	void operator delete( void* p );

protected:
	virtual void onInitialize();
	virtual void onLoadContent();
	virtual void onEvent( const ciri::WindowEvent& evt );
	virtual void onUpdate( const double deltaTime, const double elapsedTime );
	virtual void onFixedUpdate( const double deltaTime, const double elapsedTime );
	virtual void onDraw();
	virtual void onUnloadContent();

private:
	ciri::MayaCamera _camera;
	Grid _grid;
	Axis _axis;
};

#endif /* __refractdemo__ */