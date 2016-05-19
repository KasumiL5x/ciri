#include "ClippingDemo.hpp"
#include "../../common/ModelGen.hpp"
#include <cc/MatrixFunc.hpp>
#include "ClipMesh.hpp"
#include "../../common/KScene.hpp"

namespace gfx = ciri::graphics;
namespace core = ciri::core;

ClippingDemo::ClippingDemo()
	: Game(), _model(nullptr) {
	_config.width = 1280;
	_config.height = 720;
	_config.title = "ciri : Clipping Demo";
}

ClippingDemo::~ClippingDemo() {
}

void* ClippingDemo::operator new( size_t i ) {
	return _mm_malloc(i, 16);
}

void ClippingDemo::operator delete( void* p ) {
	_mm_free(p);
}

void ClippingDemo::onInitialize() {
	Game::onInitialize();

	// print driver information
	printf("Device: %s\n", graphicsDevice()->getGpuName());
	printf("API: %s\n", graphicsDevice()->getApiInfo());
	
	// configure camera
	_camera.setAspect((float)window()->getWidth() / (float)window()->getHeight());
	_camera.setPlanes(0.1f, 1000.0f);
	_camera.setYaw(300.0f);
	_camera.setPitch(18.f);
	_camera.setOffset(6.0f);
	_camera.setSensitivity(1.0f, 1.0f, 0.25f);
	_camera.setLerpStrength(10.0f);
	_camera.setTarget(cc::Vec3f(0.0f, 0.0f, 0.0f));
	_camera.resetPosition();
}

void ClippingDemo::onLoadContent() {
	Game::onLoadContent();

	// create grid
	if( !_grid.create(graphicsDevice()) ) {
		printf("Failed to create grid.\n");
	}

	// create axis
	if( !_axis.create(5.0f, graphicsDevice()) ) {
		printf("Failed to create axis.\n");
	}

	// create depth stencil state
	gfx::DepthStencilDesc depthDesc;
	_depthStencilState = graphicsDevice()->createDepthStencilState(depthDesc);
	if( nullptr == _depthStencilState ) {
		printf("Failed to create depth stencil state.\n");
	}

	// create rasterizer state
	gfx::RasterizerDesc rasterDesc;
	rasterDesc.cullMode = gfx::CullMode::None;
	rasterDesc.fillMode = gfx::FillMode::Wireframe;
	_rasterizerState = graphicsDevice()->createRasterizerState(rasterDesc);
	if( nullptr == _rasterizerState ) {
		printf("Failed to create rasterizer state.\n");
	}

	// create simple shader
	if( !_simpleShader.create(graphicsDevice()) ) {
		printf("Failed to create simple shader!\n");
	}

	// create blend state
	_blendState = graphicsDevice()->getDefaultBlendOpaque();

	// create geometric planes
	const float PLANE_SIZE = 2.0f;
	const cc::Vec3f p0 = cc::Vec3f(-0.5f*PLANE_SIZE, 0.0f,  0.5f*PLANE_SIZE);
	const cc::Vec3f p1 = cc::Vec3f(-0.5f*PLANE_SIZE, 0.0f, -0.5f*PLANE_SIZE);
	const cc::Vec3f p2 = cc::Vec3f( 0.5f*PLANE_SIZE, 0.0f, -0.5f*PLANE_SIZE);
	const cc::Vec3f p3 = cc::Vec3f( 0.5f*PLANE_SIZE, 0.0f,  0.5f*PLANE_SIZE);
	_geometricPlane = GeometricPlane(p0, p1, p2, p3);
	_geometricPlane.getXform().setPosition(cc::Vec3f(0.0f, 0.4f, 0.0f));
	_geometricPlane.getXform().setOrientation(cc::Quatf::createFromEulerAngles(0.0f, 10.0f, -15.0f));
	if( !_geometricPlane.build(graphicsDevice()) ) {
		printf("Failed to build geometric plane.\n");
	}
	_geometricPlane2 = GeometricPlane(p0, p1, p2, p3);
	_geometricPlane2.getXform().setPosition(cc::Vec3f(0.0f, -0.25f, 0.0f));
	_geometricPlane2.getXform().setOrientation(cc::Quatf::createFromEulerAngles(25.0f, 10.0f, 0.0f));
	if( !_geometricPlane2.build(graphicsDevice()) ) {
		printf("Failed to build second geometric plane.\n");
	}

	// create closed convex polyhedron
	_model = new Model();
	const float SIZE = 1.0f;
	_model->addVertex(Vertex(cc::Vec3f(-0.5f*SIZE, -0.5f*SIZE,  0.5f*SIZE), cc::Vec3f(), cc::Vec2f()));
	_model->addVertex(Vertex(cc::Vec3f( 0.5f*SIZE, -0.5f*SIZE,  0.5f*SIZE), cc::Vec3f(), cc::Vec2f()));
	_model->addVertex(Vertex(cc::Vec3f( 0.5f*SIZE,  0.5f*SIZE,  0.5f*SIZE), cc::Vec3f(), cc::Vec2f()));
	_model->addVertex(Vertex(cc::Vec3f(-0.5f*SIZE,  0.5f*SIZE,  0.5f*SIZE), cc::Vec3f(), cc::Vec2f()));
	_model->addVertex(Vertex(cc::Vec3f(-0.5f*SIZE, -0.5f*SIZE, -0.5f*SIZE), cc::Vec3f(), cc::Vec2f()));
	_model->addVertex(Vertex(cc::Vec3f( 0.5f*SIZE, -0.5f*SIZE, -0.5f*SIZE), cc::Vec3f(), cc::Vec2f()));
	_model->addVertex(Vertex(cc::Vec3f( 0.5f*SIZE,  0.5f*SIZE, -0.5f*SIZE), cc::Vec3f(), cc::Vec2f()));
	_model->addVertex(Vertex(cc::Vec3f(-0.5f*SIZE,  0.5f*SIZE, -0.5f*SIZE), cc::Vec3f(), cc::Vec2f()));
	_model->addIndex(0); _model->addIndex(1); _model->addIndex(2); // front
	_model->addIndex(2); _model->addIndex(3); _model->addIndex(0);
	_model->addIndex(1); _model->addIndex(5); _model->addIndex(6); // top
	_model->addIndex(6); _model->addIndex(2); _model->addIndex(1);
	_model->addIndex(7); _model->addIndex(6); _model->addIndex(5); // back
	_model->addIndex(5); _model->addIndex(4); _model->addIndex(7);
	_model->addIndex(4); _model->addIndex(0); _model->addIndex(3); // bottom
	_model->addIndex(3); _model->addIndex(7); _model->addIndex(4);
	_model->addIndex(4); _model->addIndex(5); _model->addIndex(1); // left
	_model->addIndex(1); _model->addIndex(0); _model->addIndex(4);
	_model->addIndex(3); _model->addIndex(2); _model->addIndex(6); // right
	_model->addIndex(6); _model->addIndex(7); _model->addIndex(3);
	_model->computeNormals();
	_model->build(graphicsDevice());

	// testing loading kscene
	KScene kScene;
	if( !kScene.readBinaryFile("C:\\Users\\daniel\\Desktop\\cube.kmdl") ) {
		printf("Failed to read binary KScene.\n");
	} else {
		kScene.printDebugInfo(false);
	}

	// create clip mesh
	cutMesh();
}

void ClippingDemo::onEvent( const core::WindowEvent& evt ) {
	Game::onEvent(evt);

	switch( evt.type ) {
		case core::WindowEvent::Resized: {
			if( graphicsDevice()->resize() != core::ErrorCode::CIRI_OK ) {
				printf("Failed to resize backbuffer.\n");
			}
			break;
		}
	}
}

void ClippingDemo::onUpdate( const double deltaTime, const double elapsedTime ) {
	Game::onUpdate(deltaTime, elapsedTime);

	// check for close w/ escape
	if( window()->hasFocus() && input()->isKeyDown(core::Key::Escape) ) {
		gtfo();
		return;
	}

	// debug print camera information
	if( window()->hasFocus() && input()->isKeyDown(core::Key::F9) && input()->wasKeyUp(core::Key::F9) ) {
		const cc::Vec3f& pos = _camera.getPosition();
		const float yaw = _camera.getYaw();
		const float pitch = _camera.getPitch();
		const float dolly = _camera.getOffset();
		const cc::Vec3f& target = _camera.getTarget();
		printf("pos(%f/%f/%f); yaw(%f); pitch(%f); dolly(%f); target(%f/%f/%f)\n", pos.x, pos.y, pos.z, yaw, pitch, dolly, target.x, target.y, target.z);
	}

	// print obj data
	if( window()->hasFocus() && input()->isKeyDown(core::Key::F5) && input()->wasKeyUp(core::Key::F5) ) {
		printf("Outputting OBJ model: %s\n", _clippedModel.exportToObj("clipped.obj") ? "success" : "failed");
		printf("Outputting OBJ model: %s\n", _model->exportToObj("source.obj") ? "success" : "failed");
	}

	// camera movement
	if( window()->hasFocus() && input()->isKeyDown(core::Key::LAlt) ) {
		// rotation
		if( input()->isMouseButtonDown(core::MouseButton::Left) ) {
			const float dx = (float)input()->mouseX() - (float)input()->lastMouseX();
			const float dy = (float)input()->mouseY() - (float)input()->lastMouseY();
			_camera.rotateYaw(-dx);
			_camera.rotatePitch(-dy);
		}
		// dolly
		if( input()->isMouseButtonDown(core::MouseButton::Right) ) {
			const float dx = (float)input()->mouseX() - (float)input()->lastMouseX();
			const float dy = (float)input()->mouseY() - (float)input()->lastMouseY();
			const float val = (fabsf(dx) > fabsf(dy)) ? dx : dy;
			_camera.dolly(val);
		}
		// pan
		if( input()->isMouseButtonDown(core::MouseButton::Middle) ) {
			const float dx = (float)input()->mouseX() - (float)input()->lastMouseX();
			const float dy = (float)input()->mouseY() - (float)input()->lastMouseY();
			_camera.pan(dx, -dy);
		}
	}

	// move geometric plane
	cc::Vec3f movement;
	if( input()->isKeyDown(core::Key::I) ) {
		movement.y += 1.0f * static_cast<float>(deltaTime);
	}
	if( input()->isKeyDown(core::Key::K) ) {
		movement.y -= 1.0f * static_cast<float>(deltaTime);
	}
	if( input()->isKeyDown(core::Key::J) ) {
		movement.x -= 1.0f * static_cast<float>(deltaTime);
	}
	if( input()->isKeyDown(core::Key::L) ) {
		movement.x += 1.0f * static_cast<float>(deltaTime);
	}
	_geometricPlane.getXform().setPosition(_geometricPlane.getXform().getPosition() + movement);

	// rotate geometric plane
	cc::Quatf rotation;
	if( input()->isKeyDown(core::Key::U) ) {
		rotation = rotation * cc::Quatf::createFromEulerAngles(0.0f, 0.0f, 45.0f * static_cast<float>(deltaTime));
	}
	if( input()->isKeyDown(core::Key::O) ) {
		rotation = rotation * cc::Quatf::createFromEulerAngles(0.0f, 0.0f, -45.0f * static_cast<float>(deltaTime));
	}
	_geometricPlane.getXform().setOrientation(_geometricPlane.getXform().getOrientation() * rotation);

	// if there's any movement, rebuild the cut mesh
	if( movement.sqrMagnitude() > 0.0f || rotation.length() != 1.0f ) {
		cutMesh();
	}
}

void ClippingDemo::onFixedUpdate( const double deltaTime, const double elapsedTime ) {
	Game::onFixedUpdate(deltaTime, elapsedTime);

	// update camera
	_camera.update((float)deltaTime);
}

void ClippingDemo::onDraw() {
	Game::onDraw();

	std::shared_ptr<gfx::IGraphicsDevice> device = graphicsDevice();

	// camera's viewproj
	const cc::Mat4f cameraViewProj = _camera.getProj() * _camera.getView();

	// clear backbuffer
	device->setClearColor(0.15f, 0.16f, 0.13f, 1.0f);
	device->clear(gfx::ClearFlags::Color | gfx::ClearFlags::Depth);

	// set default states
	device->setDepthStencilState(_depthStencilState);
	device->setRasterizerState(_rasterizerState);
	device->setBlendState(_blendState);

	// render grid
	if( _grid.isValid() ) {
		const cc::Mat4f gridXform = cameraViewProj * cc::Mat4f(1.0f);
		if( _grid.updateConstants(gridXform) ) {
			device->applyShader(_grid.getShader());
			device->setVertexBuffer(_grid.getVertexBuffer());
			device->drawArrays(gfx::PrimitiveTopology::LineList, _grid.getVertexBuffer()->getVertexCount(), 0);
		}
	}

	// //render axis
	//if( _axis.isValid() ) {
	//	const cc::Mat4f axisXform = cameraViewProj * cc::Mat4f(1.0f);
	//	if( _axis.updateConstants(axisXform) ) {
	//		device->applyShader(_axis.getShader());
	//		device->setVertexBuffer(_axis.getVertexBuffer());
	//		device->drawArrays(ciri::PrimitiveTopology::LineList, _axis.getVertexBuffer()->getVertexCount(), 0);
	//	}
	//}

	// render geometric plane
	if( _simpleShader.getShader() != nullptr && _geometricPlane.getVertexBuffer() != nullptr ) {
		device->applyShader(_simpleShader.getShader());
		_simpleShader.getConstants().world = _geometricPlane.getXform().getWorld();
		_simpleShader.getConstants().xform = cameraViewProj * _simpleShader.getConstants().world;
		_simpleShader.getMaterialConstants().hasDiffuseTexture = 0;
		_simpleShader.getMaterialConstants().diffuseColor = cc::Vec3f(1.0f, 0.0f, 1.0f);
		if( _simpleShader.updateConstants() ) {
			device->setVertexBuffer(_geometricPlane.getVertexBuffer());
			device->setIndexBuffer(_geometricPlane.getIndexBuffer());
			device->drawIndexed(gfx::PrimitiveTopology::TriangleList, _geometricPlane.getIndexBuffer()->getIndexCount());
		}
	}
	//
	if( _simpleShader.getShader() != nullptr && _geometricPlane2.getVertexBuffer() != nullptr ) {
		device->applyShader(_simpleShader.getShader());
		_simpleShader.getConstants().world = _geometricPlane2.getXform().getWorld();
		_simpleShader.getConstants().xform = cameraViewProj * _simpleShader.getConstants().world;
		_simpleShader.getMaterialConstants().hasDiffuseTexture = 0;
		_simpleShader.getMaterialConstants().diffuseColor = cc::Vec3f(1.0f, 0.0f, 1.0f);
		if( _simpleShader.updateConstants() ) {
			device->setVertexBuffer(_geometricPlane2.getVertexBuffer());
			device->setIndexBuffer(_geometricPlane2.getIndexBuffer());
			device->drawIndexed(gfx::PrimitiveTopology::TriangleList, _geometricPlane2.getIndexBuffer()->getIndexCount());
		}
	}


	// render cube
	if( _simpleShader.getShader() != nullptr && _model->isValid() ) {
		device->applyShader(_simpleShader.getShader());
		_simpleShader.getConstants().world = cc::Mat4f(1.0f);
		_simpleShader.getConstants().xform = cameraViewProj * _simpleShader.getConstants().world;
		_simpleShader.getMaterialConstants().hasDiffuseTexture = 0;
		_simpleShader.getMaterialConstants().diffuseColor = cc::Vec3f(1.0f, 1.0f, 1.0f);
		if( _simpleShader.updateConstants() ) {
			device->setVertexBuffer(_model->getVertexBuffer());
			if( _model->getIndexBuffer() != nullptr ) {
				device->setIndexBuffer(_model->getIndexBuffer());
				device->drawIndexed(gfx::PrimitiveTopology::TriangleList, _model->getIndexBuffer()->getIndexCount());
			} else {
				device->drawArrays(gfx::PrimitiveTopology::TriangleList, _model->getVertexBuffer()->getVertexCount(), 0);
			}
		}
	}

	// render clipped cube (moved up)
	if( _simpleShader.getShader() != nullptr && _clippedModel.isValid() ) {
		device->applyShader(_simpleShader.getShader());
		_simpleShader.getConstants().world = cc::math::translate<float>(cc::Vec3f(0.0f, 1.5f, 0.0f));
		_simpleShader.getConstants().xform = cameraViewProj * _simpleShader.getConstants().world;
		_simpleShader.getMaterialConstants().hasDiffuseTexture = 0;
		_simpleShader.getMaterialConstants().diffuseColor = cc::Vec3f(0.0f, 1.0f, 0.0f);
		if( _simpleShader.updateConstants() ) {
			device->setVertexBuffer(_clippedModel.getVertexBuffer());
			if( _clippedModel.getIndexBuffer() != nullptr ) {
				device->setIndexBuffer(_clippedModel.getIndexBuffer());
				device->drawIndexed(gfx::PrimitiveTopology::TriangleList, _clippedModel.getIndexBuffer()->getIndexCount());
			} else {
				device->drawArrays(gfx::PrimitiveTopology::TriangleList, _clippedModel.getVertexBuffer()->getVertexCount(), 0);
			}
		}
	}

	// present backbuffer to screen
	device->present();
}

void ClippingDemo::onUnloadContent() {
	if( _model != nullptr ) {
		delete _model;
		_model = nullptr;
	}

	// clean grid
	_grid.clean();

	// clean axis
	_axis.clean();
}

void ClippingDemo::cutMesh() {
	if( nullptr == _model ) {
		return;
	}

	// parse the input model
	ClipMesh cm(*_model);

	// perform the clip
	// test: can i clip with multiple planes? yes! yes i can! :D
	cm.clip(ClipPlane(-_geometricPlane2.getNormal(), _geometricPlane2.getConstant()));
	const ClipMesh::Result result = cm.clip(ClipPlane(_geometricPlane.getNormal(), _geometricPlane.getConstant()));
	if( ClipMesh::Result::Dissected == result ) {
		printf("Dissected\n");
	}
	if( ClipMesh::Result::Invisibubble == result ) {
		printf("Invisibubble\n");
	}
	if( ClipMesh::Result::Visible == result ) {
		printf("Visible\n");
	}
	
	//printf("Cutting complete with result: %d\n", result);

	// create a new empty model
	_clippedModel = Model();
	// convert the clipped mesh into a model
	if( cm.convert(&_clippedModel) ) {
		//printf("Vertices (%d)\n", _clippedModel.getVertices().size());
		//printf("Indices (%d)\n", _clippedModel.getIndices().size());
		_clippedModel.computeNormals();
		if( !_clippedModel.build(graphicsDevice()) ) {
			printf("Failed to build clipped model.\n");
		}
	} else {
		printf("Failed to convert model, possibly due to being fully clipped.\n");
	}
}