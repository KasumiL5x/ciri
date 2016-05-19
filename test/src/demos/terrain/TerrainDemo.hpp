#ifndef __reflections_demo__
#define __reflections_demo__

#include "../../common/Game.hpp"
#include <ciri/graphics/FPSCamera.hpp>
#include "../../common/AxisWidget.hpp"
#include "../../common/Model.hpp"
#include "../../common/ShaderPresets.hpp"
#include "../../common/HeightmapTerrain.hpp"

class TerrainDemo : public Game {
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

	virtual void onInitialize() override;
	virtual void onLoadContent() override;
	virtual void onEvent( const ciri::core::WindowEvent& evt ) override;
	virtual void onUpdate( const double deltaTime, const double elapsedTime ) override;
	virtual void onFixedUpdate( const double deltaTime, const double elapsedTime ) override;
	virtual void onDraw() override;
	virtual void onUnloadContent() override;

private:
	void drawSkybox( const cc::Mat4f& view, const cc::Mat4f& proj );

private:
	ciri::graphics::FPSCamera _camera;
	//
	std::shared_ptr<ciri::graphics::IDepthStencilState> _depthStencilState;
	std::shared_ptr<ciri::graphics::IRasterizerState> _rasterizerState;
	//
	AxisWidget _axis;
	HeightmapTerrain _terrain;

	// water stuff
	Model* _waterPlane; /**< Plane model for water. */
	std::shared_ptr<ciri::graphics::IShader> _waterShader; /**< Shader to render the water. */
	std::shared_ptr<ciri::graphics::IConstantBuffer> _waterConstantsBuffer; /**< Constant buffer for water shader. */
	WaterConstants _waterConstants; /**< Acutual constants for above buffer. */
	std::shared_ptr<ciri::graphics::ITexture2D> _waterNormalMap; /**< Normal map for water surface. */
	std::shared_ptr<ciri::graphics::ISamplerState> _waterSampler; /**< Sampler for water normal texture. */
	std::shared_ptr<ciri::graphics::IBlendState> _alphaBlendState; /**< Alpha blend state for water transparency. */
	const float WATER_HEIGHT; /**< Height of water (used for plane and for clipping). */
	std::shared_ptr<ciri::graphics::IRenderTarget2D> _waterReflectionTarget;
	std::shared_ptr<ciri::graphics::ISamplerState> _waterReflectionSampler;
	std::shared_ptr<ciri::graphics::IRenderTarget2D> _waterRefractionTarget;

	// cubemap test
	std::shared_ptr<ciri::graphics::ITextureCube> _cubemap;
	Model* _skybox;
	std::shared_ptr<ciri::graphics::IShader> _skyboxShader;
	std::shared_ptr<ciri::graphics::IConstantBuffer> _skyboxConstantsBuffer;
	SkyboxConstants _skyboxConstants;
	std::shared_ptr<ciri::graphics::ISamplerState> _skyboxSampler;
	std::shared_ptr<ciri::graphics::IDepthStencilState> _skyboxDepthState;
	//
	float _elapsedTime;

};

#endif /* __reflections_demo__ */
