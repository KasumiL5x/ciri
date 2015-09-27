#ifndef __dynamicvertexbufferdemo__
#define __dynamicvertexbufferdemo__

#include <string>
#include "../IDemo.hpp"
#include <ciri/gfx/MayaCamera.hpp>
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
	void* operator new( size_t i );
	void operator delete( void* p );

	virtual DemoConfig getConfig();
	virtual void onInitialize();
	virtual void onLoadContent();
	virtual void onEvent( ciri::WindowEvent evt );
	virtual void onUpdate( double deltaTime, double elapsedTime );
	virtual void onFixedUpdate( double deltaTime, double elapsedTime ) override;
	virtual void onDraw();
	virtual void onUnloadContent();

private:
	ciri::MayaCamera _camera;
	//
	std::shared_ptr<ciri::IDepthStencilState> _depthStencilState;
	std::shared_ptr<ciri::IRasterizerState> _rasterizerState;
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