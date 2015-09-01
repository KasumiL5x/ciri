#ifndef __reflections_demo__
#define __reflections_demo__

#include "../IDemo.hpp"
#include <ciri/gfx/FPSCamera.hpp>
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
	void drawTerrain( const cc::Mat4f& viewProj );

private:
	ciri::FPSCamera _camera;
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

	// water stuff
	Model* _waterPlane; /**< Plane model for water. */
	ciri::IShader* _waterShader; /**< Shader to render the water. */
	ciri::IConstantBuffer* _waterConstantsBuffer; /**< Constant buffer for water shader. */
	WaterConstants _waterConstants; /**< Acutual constants for above buffer. */
	ciri::ITexture2D* _waterNormalMap; /**< Normal map for water surface. */
	ciri::ISamplerState* _waterSampler; /**< Sampler for water normal texture. */
	ciri::IBlendState* _alphaBlendState; /**< Alpha blend state for water transparency. */
	const float WATER_HEIGHT; /**< Height of water (used for plane and for clipping). */
};

#endif /* __reflections_demo__ */
