#ifndef __clippingdemo__
#define __clippingdemo__

#include "../../common/Game.hpp"
#include <ciri/graphics/MayaCamera.hpp>
#include "../../common/AxisGrid.hpp"
#include "../../common/AxisWidget.hpp"
#include "../../common/Model.hpp"
#include "../../common/ShaderPresets.hpp"
#include "ClipPlane.hpp"
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
	virtual void onEvent( const ciri::core::WindowEvent& evt );
	virtual void onUpdate( const double deltaTime, const double elapsedTime );
	virtual void onFixedUpdate( const double deltaTime, const double elapsedTime );
	virtual void onDraw();
	virtual void onUnloadContent();

	void cutMesh();

private:
	ciri::graphics::MayaCamera _camera;
	AxisGrid _grid;
	AxisWidget _axis;
	std::shared_ptr<ciri::graphics::IDepthStencilState> _depthStencilState;
	std::shared_ptr<ciri::graphics::IRasterizerState> _rasterizerState;
	std::shared_ptr<ciri::graphics::IBlendState> _blendState;
	Model* _model;
	Model _clippedModel;
	SimpleShader _simpleShader;
	GeometricPlane _geometricPlane;
	GeometricPlane _geometricPlane2;
};

#endif /* __clippingdemo__ */
