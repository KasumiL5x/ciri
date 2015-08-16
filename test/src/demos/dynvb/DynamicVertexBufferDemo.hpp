#ifndef __dynamicvertexbufferdemo__
#define __dynamicvertexbufferdemo__

#include <string>
#include "../IDemo.hpp"
#include <ciri/gfx/MayaCamera.hpp>
#include <ciri/input/Input.hpp>
#include "../../common/Grid.hpp"
#include "../../common/Axis.hpp"
#include "../../common/Model.hpp"
#include "../../common/ShaderPresets.hpp"

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
	ciri::Window* _window;
	std::string _shaderExtension;
	//
	ciri::MayaCamera _camera;
	//
	ciri::KeyboardState _prevKeyState;
	ciri::MouseState _prevMouseState;
	//
	Grid _grid;
	Axis _axis;
	//
	SimpleShader _simpleShader;
	Model _flagpole;
};

#endif /* __dynamicvertexbufferdemo__ */