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
#include "OpenCloth.hpp"

class DynamicVertexBufferDemo : public IDemo {
private:
	struct FlagVertex {
		cc::Vec3f position;
	};

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
	//
	ciri::MayaCamera _camera;
	//
	ciri::KeyboardState _prevKeyState;
	ciri::MouseState _prevMouseState;
	//
	ciri::IDepthStencilState* _depthStencilState;
	ciri::IRasterizerState* _rasterizerState;
	//
	Grid _grid;
	Axis _axis;
	//
	SimpleShader _simpleShader;
	Model _flagpole;
	//
	OpenCloth _cloth;
	bool _clothRunning;
};

#endif /* __dynamicvertexbufferdemo__ */