#ifndef __refractdemo__
#define __refractdemo__

#include "../../framework/Game.hpp"
#include <ciri/gfx/MayaCamera.hpp>
#include "../../common/Grid.hpp"
#include "../../common/Axis.hpp"
#include "../../common/Model.hpp"
#include "../../common/ModelGen.hpp"

__declspec(align(16))
struct RefractVertexConstants {
	cc::Mat4f world;
	cc::Mat4f xform;
	cc::Vec3f campos;
};

class RefractDemo : public Game {
public:
	RefractDemo();
	virtual ~RefractDemo();
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

private:
	bool loadShaders();
	void unloadShaders();

private:
	ciri::MayaCamera _camera;
	Grid _grid;
	Axis _axis;
	std::shared_ptr<ciri::IDepthStencilState> _depthStencilState;
	std::shared_ptr<ciri::IRasterizerState> _rasterizerState;
	std::shared_ptr<ciri::IShader> _refractShader;
	std::shared_ptr<ciri::IConstantBuffer> _refractVertexConstantBuffer;
	RefractVertexConstants _refractVertexConstants;
	Model* _model;
	//
	std::shared_ptr<ciri::ITextureCube> _cubemap;
	std::shared_ptr<ciri::ISamplerState> _cubemapSampler;
	std::shared_ptr<ciri::ITexture3D> _texture3D;
};

#endif /* __refractdemo__ */