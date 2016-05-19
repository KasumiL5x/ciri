#ifndef __parallax_demo__
#define __parallax_demo__

// http://www.learnopengl.com/#!Advanced-Lighting/Parallax-Mapping
// http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/a-closer-look-at-parallax-occlusion-mapping-r3262
// http://sunandblackcat.com/tipFullView.php?topicid=28

#include <ciri/Game.hpp>
#include <ciri/graphics/MayaCamera.hpp>
#include "../../common/AxisGrid.hpp"
#include "../../common/AxisWidget.hpp"
#include "../../common/Model.hpp"

__declspec(align(16))
struct ParallaxVertexConstants {
	cc::Mat4f world;
	cc::Mat4f xform;
	cc::Vec3f campos;
};

class ParallaxDemo : public ciri::App {
public:
	ParallaxDemo();
	virtual ~ParallaxDemo();
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
	AxisGrid _grid;
	AxisWidget _axis;
	std::shared_ptr<ciri::IDepthStencilState> _depthStencilState;
	std::shared_ptr<ciri::IRasterizerState> _rasterizerState;
	Model* _model;
	std::shared_ptr<ciri::IShader> _parallaxShader;
	std::shared_ptr<ciri::IConstantBuffer> _parallaxVertexConstantBuffer;
	ParallaxVertexConstants _parallaxVertexConstants;
	std::shared_ptr<ciri::ITexture2D> _diffuseTexture;
	std::shared_ptr<ciri::ITexture2D> _normalTexture;
	std::shared_ptr<ciri::ITexture2D> _parallaxTexture;
	std::shared_ptr<ciri::ISamplerState> _parallaxSampler;
};

#endif /* __parallax_demo__ */