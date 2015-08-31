#ifndef __reflections_demo__
#define __reflections_demo__

#include "../IDemo.hpp"
#include <ciri/gfx/MayaCamera.hpp>
#include <ciri/input/Input.hpp>
#include "../../common/Axis.hpp"
#include "../../common/Model.hpp"
#include "../../common/ShaderPresets.hpp"
#include "../../common/HeightmapTerrain.hpp"

class TerrainDemo : public IDemo {
private:
	struct WaterConstants {
		cc::Mat4f world;
		cc::Mat4f worldview;
		cc::Mat4f xform;
		cc::Vec3f campos;
	};

public:
	TerrainDemo();
	virtual ~TerrainDemo();

	virtual DemoConfig getConfig();
	virtual void onInitialize();
	virtual void onLoadContent();
	virtual void onEvent( ciri::WindowEvent evt );
	virtual void onUpdate( double deltaTime, double elapsedTime );
	virtual void onDraw();
	virtual void onUnloadContent();

private:
	ciri::MayaCamera _camera;
	//
	ciri::KeyboardState _prevKeyState;
	ciri::MouseState _prevMouseState;
	//
	ciri::IDepthStencilState* _depthStencilState;
	ciri::IRasterizerState* _rasterizerState;
	//
	Axis _axis;
	SimpleShader _simpleShader;
	HeightmapTerrain _terrain;
	Model* _waterPlane;
	ciri::IShader* _waterShader;
	ciri::IConstantBuffer* _waterConstantsBuffer;
	WaterConstants _waterConstants;
	ciri::ITexture2D* _waterNormalMap;
	ciri::ISamplerState* _waterSampler;
	ciri::IBlendState* _alphaBlendState;
};

#endif /* __reflections_demo__ */
