#include "playground.hpp"
#include "../../common/ModelGen.hpp"

Playground::Playground()
	: App(), _waterModel(nullptr) {
	_config.width = 1280;
	_config.height = 720;
	_config.title = "ciri : Playground";
}

Playground::~Playground() {
}

void Playground::onInitialize() {
	App::onInitialize();

	// print driver information
	printf("Device: %s\n", graphicsDevice()->getGpuName());
	printf("API: %s\n", graphicsDevice()->getApiInfo());

	_camera.setAspect((float)window()->getWidth() / (float)window()->getHeight());
	_camera.setPlanes(0.1f, 10000.0f);
	//_camera.setSensitivity(100.0f, 1.0f);
	//_camera.setPosition(cc::Vec3f(-45.5f, 33.0f, 32.0f));
	//_camera.setYaw(322.0f);
	//_camera.setPitch(-30.0f);
}

void Playground::onLoadContent() {
	App::onLoadContent();

	// create water plane
	_waterModel = modelgen::createPlane(graphicsDevice(), 5000.0f, 5000.0f, 200, 200, 1.0f, 1.0f, false, false);

	// shader stuff
	loadShaders();

	// create states
	_rasterState = graphicsDevice()->getDefaultRasterNone();
	_blendState = graphicsDevice()->getDefaultBlendAdditive();

	// hook up player controller
	_plane = std::make_shared<Plane>();
	_playerPlaneController = std::make_shared<PlayerPlaneController>(_plane, input());
	_plane->setController(_playerPlaneController);

	// load plane model
	if( !_planeModel.addFromObj("data/demos/playground/frigate.obj") ) {
		printf("Failed to load plane model.\n");
	}
	if( !_planeModel.build(graphicsDevice()) ) {
		printf("Failed to build plane model.\n");
	}
}

void Playground::onEvent(const ciri::WindowEvent& evt) {
	App::onEvent(evt);
}

void Playground::onUpdate(const double deltaTime, const double elapsedTime) {
	App::onUpdate(deltaTime, elapsedTime);

	if( !window()->hasFocus() ) {
		return;
	}
	
	if( input()->isKeyDown(ciri::Key::F5) && input()->wasKeyUp(ciri::Key::F5) ) {
		_waterShader->destroy();
		loadShaders();
		printf("Reloaded shaders.\n");
	}

	// update shader constant for elapsed time
	_waterConstants.ElapsedTime = elapsedTime;

	// update player controller
	_playerPlaneController->update(deltaTime, elapsedTime);

	// update camera stuff
	_cameraOrientation = cc::Quatf::lerp(_cameraOrientation, _plane->orientation(), 0.001f);
	const cc::Mat4f cameraOrientMatrix = cc::Quatf::createMatrixFromQuaternion(_cameraOrientation);
	cc::Vec3f camPos(0.0f, 10.0f, 175.f);
	camPos = (cameraOrientMatrix*cc::Vec4f(camPos, 1.0f)).truncated();
	camPos += _plane->position();
	cc::Vec3f camUp(0.0f, 1.0f, 0.0f);
	camUp = (cameraOrientMatrix*cc::Vec4f(camUp, 1.0f)).truncated();
	_camera.setPosition(camPos);
	_camera.setUp(camUp);
	_camera.setTarget(_plane->position());
}

void Playground::onFixedUpdate(const double deltaTime, const double elapsedTime) {
	App::onFixedUpdate(deltaTime, elapsedTime);
}

void Playground::onDraw() {
	App::onDraw();

	const cc::Mat4f cameraViewProj = _camera.getProj() * _camera.getView();

	const auto device = graphicsDevice();
	device->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);

	device->setRasterizerState(_rasterState);
	//device->setBlendState(_blendState);

	if( _waterModel->isValid() && _waterShader->isValid() ) {
		// update constants
		_waterConstants.world = cc::Mat4f(1.0f);
		_waterConstants.xform = cameraViewProj * _waterConstants.world;
		_waterConstants.CamPos = _camera.getPosition();
		if( ciri::failed(_waterConstantsBuffer->setData(sizeof(PGWaterConstants), &_waterConstants)) ) {
			printf("Failed to update water constants.\n");
		} else {
			device->applyShader(_waterShader);
			device->setVertexBuffer(_waterModel->getVertexBuffer());
			device->setIndexBuffer(_waterModel->getIndexBuffer());
			device->drawIndexed(ciri::PrimitiveTopology::TriangleList, _waterModel->getIndexBuffer()->getIndexCount());		
		}
	}

	if( _planeModel.isValid() && _basicShader->isValid() ) {
		// update constants
		_basicConstants.world = _plane->world();
		_basicConstants.xform = cameraViewProj * _basicConstants.world;
		_basicConstants.campos = _camera.getPosition();
		if( ciri::failed(_basicConstantBuffer->setData(sizeof(BasicConstants), &_basicConstants)) ) {
			printf("Failed to update basic constants.\n");
		} else {
			device->applyShader(_basicShader);
			device->setVertexBuffer(_planeModel.getVertexBuffer());
			if( _planeModel.getIndexBuffer() != nullptr ) {
				device->setIndexBuffer(_planeModel.getIndexBuffer());
				device->drawIndexed(ciri::PrimitiveTopology::TriangleList, _planeModel.getIndexBuffer()->getIndexCount());
			} else {
				device->drawArrays(ciri::PrimitiveTopology::TriangleList, _planeModel.getVertexBuffer()->getVertexCount(), 0);
			}
		}
	}

	device->present();
}

void Playground::onUnloadContent() {
	App::onUnloadContent();

	if( _waterModel != nullptr ) {
		delete _waterModel;
		_waterModel = nullptr;
	}
}

void Playground::loadShaders() {
	// load water shader
	if( _waterShader == nullptr ) {
		_waterShader = graphicsDevice()->createShader();
		_waterShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Position, 0));
		_waterShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Normal, 0));
		_waterShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float4, ciri::VertexUsage::Tangent, 0));
		_waterShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float2, ciri::VertexUsage::Texcoord, 0));
	}
	const std::string shaderExt = graphicsDevice()->getShaderExt();
	const std::string waterVS = "data/demos/playground/water_vs" + shaderExt;
	const std::string waterGS = "data/demos/playground/water_gs" + shaderExt;
	const std::string waterPS = "data/demos/playground/water_ps" + shaderExt;
	if( ciri::failed(_waterShader->loadFromFile(waterVS.c_str(), waterGS.c_str(), waterPS.c_str())) ) {
		printf("Failed to load water shader.\n");
		for( auto& err : _waterShader->getErrors() ) {
			printf("%s\n", err.msg.c_str());
		}
	}
	
	// create and attach water constants
	if( _waterConstantsBuffer == nullptr ) {
		_waterConstantsBuffer = graphicsDevice()->createConstantBuffer();
	}
	if( ciri::failed(_waterConstantsBuffer->setData(sizeof(PGWaterConstants), &_waterConstants)) ) {
		printf("Failed to set water constants.\n");
	} else {
		if( ciri::failed(_waterShader->addConstants(_waterConstantsBuffer, "WaterConstants", ciri::ShaderStage::Vertex)) ) {
			printf("Failed to apply water constants to shader.\n");
		}
	}

	//
	// basic shader
	//
	if( nullptr == _basicShader ) {
		_basicShader = graphicsDevice()->createShader();
		_basicShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Position, 0));
		_basicShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Normal, 0));
		_basicShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float4, ciri::VertexUsage::Tangent, 0));
		_basicShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float2, ciri::VertexUsage::Texcoord, 0));
	}
	const std::string basicVS = "data/demos/playground/basic_vs" + shaderExt;
	const std::string basicPS = "data/demos/playground/basic_ps" + shaderExt;
	if( ciri::failed(_basicShader->loadFromFile(basicVS.c_str(), nullptr, basicPS.c_str())) ) {
		printf("Failed to load basic shader:\n");
		for( auto& err : _basicShader->getErrors() ) {
			printf("%s\n", err.msg.c_str());
		}
	}
	if( nullptr == _basicConstantBuffer ) {
		_basicConstantBuffer = graphicsDevice()->createConstantBuffer();
		if( ciri::failed(_basicConstantBuffer->setData(sizeof(BasicConstants), &_basicConstants)) ) {
			printf("Failed to set basic constants.\n");
		}
	}
	if( ciri::failed(_basicShader->addConstants(_basicConstantBuffer, "BasicConstants", ciri::ShaderStage::Vertex)) ) {
		printf("Failed to assign basic constants to shader.\n");
	}
}