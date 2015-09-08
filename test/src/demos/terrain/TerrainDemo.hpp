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
	_declspec(align(16))
	struct WaterConstants {
		cc::Mat4f world;
		cc::Mat4f xform;
		cc::Mat4f reflectedViewProj;
		cc::Vec3f campos;
		float time;
	};

	_declspec(align(16))
	struct SkyboxConstants {
		cc::Mat4f view;
		cc::Mat4f proj;
	};

public:
	TerrainDemo();
	virtual ~TerrainDemo();

	void* operator new( size_t i ) {
		return _mm_malloc(i, 16);
	}
	void operator delete( void* p ) {
		_mm_free(p);
	}

	virtual DemoConfig getConfig();
	virtual void onInitialize();
	virtual void onLoadContent();
	virtual void onEvent( ciri::WindowEvent evt );
	virtual void onUpdate( double deltaTime, double elapsedTime );
	virtual void onDraw();
	virtual void onUnloadContent();

private:
	void drawSkybox( const cc::Mat4f& view, const cc::Mat4f& proj );

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
	ciri::IRenderTarget2D* _waterReflectionTarget;
	ciri::ISamplerState* _waterReflectionSampler;
	ciri::IRenderTarget2D* _waterRefractionTarget;

	// cubemap test
	ciri::ITextureCube* _cubemap;
	Model* _skybox;
	ciri::IShader* _skyboxShader;
	ciri::IConstantBuffer* _skyboxConstantsBuffer;
	SkyboxConstants _skyboxConstants;
	ciri::ISamplerState* _skyboxSampler;
	ciri::IDepthStencilState* _skyboxDepthState;
	//
	float _elapsedTime;

};

#endif /* __reflections_demo__ */
