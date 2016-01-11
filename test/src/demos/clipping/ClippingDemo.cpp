#include "ClippingDemo.hpp"
#include "../../common/ModelGen.hpp"
#include <cc/MatrixFunc.hpp>
#include "ClipMesh.hpp"

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

	// configure the cutting plane
	_cuttingPlane.normal = cc::Vec3f(0.0f, 1.0f, 0.0f).normalized();
	_cuttingPlane.distance = 0.0f;
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
	ciri::DepthStencilDesc depthDesc;
	_depthStencilState = graphicsDevice()->createDepthStencilState(depthDesc);
	if( nullptr == _depthStencilState ) {
		printf("Failed to create depth stencil state.\n");
	}

	// create rasterizer state
	ciri::RasterizerDesc rasterDesc;
	rasterDesc.cullMode = ciri::CullMode::None;
	//rasterDesc.fillMode = ciri::FillMode::Wireframe;
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

	// create geometric plane
	const float PLANE_SIZE = 2.0f;
	const cc::Vec3f p0 = cc::Vec3f(-0.5f*PLANE_SIZE, 0.0f,  0.5f*PLANE_SIZE);
	const cc::Vec3f p1 = cc::Vec3f(-0.5f*PLANE_SIZE, 0.0f, -0.5f*PLANE_SIZE);
	const cc::Vec3f p2 = cc::Vec3f( 0.5f*PLANE_SIZE, 0.0f, -0.5f*PLANE_SIZE);
	const cc::Vec3f p3 = cc::Vec3f( 0.5f*PLANE_SIZE, 0.0f,  0.5f*PLANE_SIZE);
	_geometricPlane = GeometricPlane(p0, p1, p2, p3);
	if( !_geometricPlane.build(graphicsDevice()) ) {
		printf("Failed to build geometric plane.\n");
	}
	//_geometricPlane.getXform().setOrientation(_geometricPlane.getXform().getOrientation() * cc::Quatf::createFromEulerAngles(45.0f, 0.0f, 0.0f));
	//_geometricPlane.getXform().setOrientation(_geometricPlane.getXform().getOrientation() * cc::Quatf::createFromEulerAngles(0.0f, 0.0f, 20.0f));

	// get cut plane from geometric plane
	_cuttingPlane.normal = _geometricPlane.getNormal();
	_cuttingPlane.distance = _geometricPlane.getConstant();

	// create closed convex polyhedron
	_model = new Model();
	_model->addVertex(Vertex(cc::Vec3f(-0.5f, -0.5f, 0.0f), cc::Vec3f(), cc::Vec2f()));
	_model->addVertex(Vertex(cc::Vec3f( 0.5f, -0.5f, 0.0f), cc::Vec3f(), cc::Vec2f()));
	_model->addVertex(Vertex(cc::Vec3f(-0.5f,  0.5f, 0.0f), cc::Vec3f(), cc::Vec2f()));
	_model->addVertex(Vertex(cc::Vec3f( 0.5f,  0.5f, 0.0f), cc::Vec3f(), cc::Vec2f()));
	_model->addVertex(Vertex(cc::Vec3f(-0.5f, -0.5f, -0.5f), cc::Vec3f(), cc::Vec2f()));
	_model->addVertex(Vertex(cc::Vec3f( 0.5f, -0.5f, -0.5f), cc::Vec3f(), cc::Vec2f()));
	_model->addVertex(Vertex(cc::Vec3f(-0.5f, 0.5f, -0.5f), cc::Vec3f(), cc::Vec2f()));
	_model->addVertex(Vertex(cc::Vec3f( 0.5f, 0.5f, -0.5f), cc::Vec3f(), cc::Vec2f()));
	_model->addIndex(0); _model->addIndex(1); _model->addIndex(2);//front
	_model->addIndex(1); _model->addIndex(3); _model->addIndex(2);//front
	_model->addIndex(0); _model->addIndex(1); _model->addIndex(4);//bottom
	_model->addIndex(1); _model->addIndex(5); _model->addIndex(4);//bottom
	_model->addIndex(5); _model->addIndex(7); _model->addIndex(6);//back
	_model->addIndex(6); _model->addIndex(4); _model->addIndex(5);//back
	_model->addIndex(0); _model->addIndex(2); _model->addIndex(4);//left
	_model->addIndex(2); _model->addIndex(6); _model->addIndex(4);//left
	_model->addIndex(1); _model->addIndex(5); _model->addIndex(3);//right
	_model->addIndex(5); _model->addIndex(7); _model->addIndex(3);//right
	_model->addIndex(3); _model->addIndex(7); _model->addIndex(6);//top
	_model->addIndex(2); _model->addIndex(3); _model->addIndex(6);//top
	_model->computeNormals();
	_model->build(graphicsDevice());

	// create clip mesh
	ClipMesh cm(*_model);
	const int result = cm.clip(_cuttingPlane);
	printf("Cutting complete with result: %d\n", result);
	cm.printDebug(false);
	cm.convert(&_clippedModel);
	//_clippedModel = cm.convert();
	_clippedModel.computeNormals();
	printf("Vertices (%d):\n", _clippedModel.getVertices().size());
	for( int i = 0 ; i < _clippedModel.getVertices().size(); ++i ) {
	}
	printf("Indices (%d):\n", _clippedModel.getIndices().size());
	for( int i = 0; i < _clippedModel.getIndices().size(); ++i ) {
		//printf("\t[%d]: %d\n", i, _clippedModel.getIndices()[i]);
	}
	if( !_clippedModel.build(graphicsDevice()) ) {
		printf("Failed to build clipped model.\n");
	}
}

void ClippingDemo::onEvent( const ciri::WindowEvent& evt ) {
	Game::onEvent(evt);

	switch( evt.type ) {
		case ciri::WindowEvent::Resized: {
			if( graphicsDevice()->resize() != ciri::ErrorCode::CIRI_OK ) {
				printf("Failed to resize backbuffer.\n");
			}
			break;
		}
	}
}

void ClippingDemo::onUpdate( const double deltaTime, const double elapsedTime ) {
	Game::onUpdate(deltaTime, elapsedTime);

	// check for close w/ escape
	if( window()->hasFocus() && input()->isKeyDown(ciri::Key::Escape) ) {
		gtfo();
		return;
	}

	// debug print camera information
	if( window()->hasFocus() && input()->isKeyDown(ciri::Key::F9) && input()->wasKeyUp(ciri::Key::F9) ) {
		const cc::Vec3f& pos = _camera.getPosition();
		const float yaw = _camera.getYaw();
		const float pitch = _camera.getPitch();
		const float dolly = _camera.getOffset();
		const cc::Vec3f& target = _camera.getTarget();
		printf("pos(%f/%f/%f); yaw(%f); pitch(%f); dolly(%f); target(%f/%f/%f)\n", pos.x, pos.y, pos.z, yaw, pitch, dolly, target.x, target.y, target.z);
	}

	// print obj data
	if( window()->hasFocus() && input()->isKeyDown(ciri::Key::F5) && input()->wasKeyUp(ciri::Key::F5) ) {
		printf("Outputting OBJ model: %s\n", _clippedModel.exportToObj("clipped.obj") ? "success" : "failed");
		printf("Outputting OBJ model: %s\n", _model->exportToObj("source.obj") ? "success" : "failed");
	}

	// camera movement
	if( window()->hasFocus() && input()->isKeyDown(ciri::Key::LAlt) ) {
		// rotation
		if( input()->isMouseButtonDown(ciri::MouseButton::Left) ) {
			const float dx = (float)input()->mouseX() - (float)input()->lastMouseX();
			const float dy = (float)input()->mouseY() - (float)input()->lastMouseY();
			_camera.rotateYaw(-dx);
			_camera.rotatePitch(-dy);
		}
		// dolly
		if( input()->isMouseButtonDown(ciri::MouseButton::Right) ) {
			const float dx = (float)input()->mouseX() - (float)input()->lastMouseX();
			const float dy = (float)input()->mouseY() - (float)input()->lastMouseY();
			const float val = (fabsf(dx) > fabsf(dy)) ? dx : dy;
			_camera.dolly(val);
		}
		// pan
		if( input()->isMouseButtonDown(ciri::MouseButton::Middle) ) {
			const float dx = (float)input()->mouseX() - (float)input()->lastMouseX();
			const float dy = (float)input()->mouseY() - (float)input()->lastMouseY();
			_camera.pan(dx, -dy);
		}
	}

	// move geometric plane
	cc::Vec3f movement;
	if( input()->isKeyDown(ciri::Key::I) ) {
		movement.y += 1.0f * deltaTime;
	}
	if( input()->isKeyDown(ciri::Key::K) ) {
		movement.y -= 1.0f * deltaTime;
	}
	if( input()->isKeyDown(ciri::Key::J) ) {
		movement.x -= 1.0f * deltaTime;
	}
	if( input()->isKeyDown(ciri::Key::L) ) {
		movement.x += 1.0f * deltaTime;
	}
	_geometricPlane.getXform().setPosition(_geometricPlane.getXform().getPosition() + movement);

	// rotate geometric plane
	cc::Quatf rotation;
	if( input()->isKeyDown(ciri::Key::U) ) {
		rotation = rotation * cc::Quatf::createFromEulerAngles(0.0f, 0.0f, 45.0f * deltaTime);
	}
	if( input()->isKeyDown(ciri::Key::O) ) {
		rotation = rotation * cc::Quatf::createFromEulerAngles(0.0f, 0.0f, -45.0f * deltaTime);
	}
	_geometricPlane.getXform().setOrientation(_geometricPlane.getXform().getOrientation() * rotation);
	

	// rebuild cutting plane from new geometric plane position
	_cuttingPlane.normal = _geometricPlane.getNormal();
	_cuttingPlane.distance = _geometricPlane.getConstant();
	//printf("N(%f, %f, %f); D(%f)\n", _cuttingPlane.normal.x, _cuttingPlane.normal.y, _cuttingPlane.normal.z, _cuttingPlane.distance);

	// if there's any movement, rebuild the cut mesh
	if( movement.sqrMagnitude() > 0.0f || rotation.length() != 1.0f ) {
		ClipMesh cm(*_model);
		const int result = cm.clip(_cuttingPlane);
		printf("Cutting complete with result: %d\n", result);
		_clippedModel = Model();
		cm.convert(&_clippedModel);
		//_clippedModel = cm.convert();
		_clippedModel.computeNormals();
		printf("Vertices (%d)\n", _clippedModel.getVertices().size());
		printf("Indices (%d)\n", _clippedModel.getIndices().size());
		if( !_clippedModel.build(graphicsDevice()) ) {
			printf("Failed to build clipped model.\n");
		}
	}
}

void ClippingDemo::onFixedUpdate( const double deltaTime, const double elapsedTime ) {
	Game::onFixedUpdate(deltaTime, elapsedTime);

	// update camera
	_camera.update((float)deltaTime);
}

void ClippingDemo::onDraw() {
	Game::onDraw();

	std::shared_ptr<ciri::IGraphicsDevice> device = graphicsDevice();

	// camera's viewproj
	const cc::Mat4f cameraViewProj = _camera.getProj() * _camera.getView();

	// clear backbuffer
	device->setClearColor(0.15f, 0.16f, 0.13f, 1.0f);
	device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);

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
			device->drawArrays(ciri::PrimitiveTopology::LineList, _grid.getVertexBuffer()->getVertexCount(), 0);
		}
	}

	 //render axis
	if( _axis.isValid() ) {
		const cc::Mat4f axisXform = cameraViewProj * cc::Mat4f(1.0f);
		if( _axis.updateConstants(axisXform) ) {
			device->applyShader(_axis.getShader());
			device->setVertexBuffer(_axis.getVertexBuffer());
			device->drawArrays(ciri::PrimitiveTopology::LineList, _axis.getVertexBuffer()->getVertexCount(), 0);
		}
	}

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
			device->drawIndexed(ciri::PrimitiveTopology::TriangleList, _geometricPlane.getIndexBuffer()->getIndexCount());
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
				device->drawIndexed(ciri::PrimitiveTopology::TriangleList, _model->getIndexBuffer()->getIndexCount());
			} else {
				device->drawArrays(ciri::PrimitiveTopology::TriangleList, _model->getVertexBuffer()->getVertexCount(), 0);
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
				device->drawIndexed(ciri::PrimitiveTopology::TriangleList, _clippedModel.getIndexBuffer()->getIndexCount());
			} else {
				device->drawArrays(ciri::PrimitiveTopology::TriangleList, _clippedModel.getVertexBuffer()->getVertexCount(), 0);
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