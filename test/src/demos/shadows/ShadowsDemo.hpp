#ifndef __ShadowsDemo__
#define __ShadowsDemo__

#include <memory>
#include <ciri/Game.hpp>
#include "../../common/Model.hpp"
#include "Light.hpp"

__declspec(align(16))
struct SpotlightConstants {
	cc::Mat4f world;
	cc::Mat4f xform;
	cc::Vec3f campos;
	float LightCosInner;
	cc::Vec3f LightPosition;
	float LightCosOuter;
	cc::Vec3f LightDirection;
	float LightIntensity;
	cc::Vec3f LightColor;
	float LightRange;
	cc::Mat4f lightViewProj;
	int CastShadows;
};

__declspec(align(16))
struct DirectionalConstants {
	cc::Mat4f world;
	cc::Mat4f xform;
	cc::Vec3f campos;
	float LightIntensity;
	cc::Vec3f LightDirection;
	int CastShadows;
	cc::Vec3f LightColor;
	float pad0;
	cc::Mat4f lightViewProj;
};

__declspec(align(16))
struct DepthConstants {
	cc::Mat4f xform;
};

class ShadowsDemo : public ciri::App {
public:
	ShadowsDemo();
	virtual ~ShadowsDemo();

	virtual void onInitialize() override;
	virtual void onLoadContent() override;
	virtual void onEvent(const ciri::WindowEvent& evt) override;
	virtual void onUpdate(const double deltaTime, const double elapsedTime) override;
	virtual void onFixedUpdate(const double deltaTime, const double elapsedTime) override;
	virtual void onDraw() override;
	virtual void onUnloadContent() override;

private:
	void loadShaders();

private:
	ciri::FPSCamera _camera;
	std::vector<std::shared_ptr<Model>> _models;
	std::vector<Light> _lights;
	std::shared_ptr<ciri::IShader> _spotlightShader;
	std::shared_ptr<ciri::IConstantBuffer> _spotlightConstantsBuffer;
	std::shared_ptr<ciri::IShader> _directionalShader;
	std::shared_ptr<ciri::IConstantBuffer> _directionalConstantsBuffer;
	DirectionalConstants _directionalConstants;
	SpotlightConstants _spotlightConstants;
	std::shared_ptr<ciri::IRasterizerState> _rasterState;
	std::shared_ptr<Model> _ground;
	std::shared_ptr<Model> _helicopterBody;
	std::shared_ptr<Model> _helicopterBlades;
	std::shared_ptr<Model> _helicopterTail;
	Light* _cameraLight;
	bool _lightFollowCamera;
	std::shared_ptr<ciri::IBlendState> _additiveBlendState;
	std::shared_ptr<ciri::IRenderTarget2D> _shadowTarget;
	std::shared_ptr<ciri::ISamplerState> _shadowSampler;
	std::shared_ptr<ciri::IShader> _depthShader;
	std::shared_ptr<ciri::IConstantBuffer> _depthConstantsBuffer;
	DepthConstants _depthConstants;
	bool _animateObjects;
};

#endif