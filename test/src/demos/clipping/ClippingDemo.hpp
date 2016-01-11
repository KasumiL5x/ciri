#ifndef __clippingdemo__
#define __clippingdemo__

#include "../../framework/Game.hpp"
#include "../../framework/Game.hpp"
#include <ciri/gfx/MayaCamera.hpp>
#include "../../common/Grid.hpp"
#include "../../common/Axis.hpp"
#include "../../common/Model.hpp"
#include "../../common/ShaderPresets.hpp"
#include "Plane.hpp"
#include "../../common/GeometricPlane.hpp"

class ClippingDemo : public Game {
public:
	ClippingDemo();
	virtual ~ClippingDemo();
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

	void cutMesh();

private:
	ciri::MayaCamera _camera;
	Grid _grid;
	Axis _axis;
	std::shared_ptr<ciri::IDepthStencilState> _depthStencilState;
	std::shared_ptr<ciri::IRasterizerState> _rasterizerState;
	std::shared_ptr<ciri::IBlendState> _blendState;
	Model* _model;
	Model _clippedModel;
	SimpleShader _simpleShader;
	Plane _cuttingPlane;
	GeometricPlane _geometricPlane;
};

#endif /* __clippingdemo__ */
