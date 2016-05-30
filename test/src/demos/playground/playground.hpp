#ifndef __playground__
#define __playground__

#include <ciri/Game.hpp>
#include "../../common/Model.hpp"
#include <cc/Mat4.hpp>
#include "Plane.hpp"
#include "PlayerPlaneController.hpp"
#include <cc/Quaternion.hpp>

__declspec(align(16))
struct PGWaterConstants {
	cc::Mat4f world;
	cc::Mat4f xform;
	float ElapsedTime;
	cc::Vec3f CamPos;
};

__declspec(align(16))
struct BasicConstants {
	cc::Mat4f world;
	cc::Mat4f xform;
	cc::Vec3f campos;
};

class Playground : public ciri::App {
public:
	Playground();
	virtual ~Playground();

	virtual void onInitialize();
	virtual void onLoadContent();
	virtual void onEvent(const ciri::WindowEvent& evt);
	virtual void onUpdate(const double deltaTime, const double elapsedTime);
	virtual void onFixedUpdate(const double deltaTime, const double elapsedTime);
	virtual void onDraw();
	virtual void onUnloadContent();

private:
	void loadShaders();

private:
	ciri::Camera _camera;
	Model* _waterModel;
	std::shared_ptr<ciri::IShader> _waterShader;
	std::shared_ptr<ciri::IConstantBuffer> _waterConstantsBuffer;
	PGWaterConstants _waterConstants;
	std::shared_ptr<ciri::IRasterizerState> _rasterState;
	std::shared_ptr<ciri::IBlendState> _blendState;
	std::shared_ptr<Plane> _plane;
	std::shared_ptr<PlayerPlaneController> _playerPlaneController;
	Model _planeModel;
	std::shared_ptr<ciri::IShader> _basicShader;
	std::shared_ptr<ciri::IConstantBuffer> _basicConstantBuffer;
	BasicConstants _basicConstants;
	cc::Quatf _cameraOrientation;
};

#endif