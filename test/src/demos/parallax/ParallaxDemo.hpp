#ifndef __parallax_demo__
#define __parallax_demo__

// http://www.learnopengl.com/#!Advanced-Lighting/Parallax-Mapping
// http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/a-closer-look-at-parallax-occlusion-mapping-r3262
// http://sunandblackcat.com/tipFullView.php?topicid=28

#include "../../framework/Game.hpp"
#include <ciri/gfx/MayaCamera.hpp>
#include "../../common/Grid.hpp"
#include "../../common/Axis.hpp"

class ParallaxDemo : public Game {
public:
	ParallaxDemo();
	virtual ~ParallaxDemo();
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
	bool loadShaders();
	void unloadShaders();

private:
	ciri::MayaCamera _camera;
	Grid _grid;
	Axis _axis;
	std::shared_ptr<ciri::IDepthStencilState> _depthStencilState;
	std::shared_ptr<ciri::IRasterizerState> _rasterizerState;
};

#endif /* __parallax_demo__ */