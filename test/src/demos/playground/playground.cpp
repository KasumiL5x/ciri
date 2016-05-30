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
	_camera.setSensitivity(100.0f, 1.0f);
	_camera.setPosition(cc::Vec3f(-45.5f, 33.0f, 32.0f));
	_camera.setYaw(322.0f);
	_camera.setPitch(-30.0f);
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
}

void Playground::onEvent(const ciri::WindowEvent& evt) {
	App::onEvent(evt);
}

void Playground::onUpdate(const double deltaTime, const double elapsedTime) {
	App::onUpdate(deltaTime, elapsedTime);

	if( !window()->hasFocus() ) {
		return;
	}
	
	if( input()->isKeyDown(ciri::Key::F9) && input()->wasKeyUp(ciri::Key::F9) ) {
		const cc::Vec3f& pos = _camera.getPosition();
		const float yaw = _camera.getYaw();
		const float pitch = _camera.getPitch();
		printf("Camera Information\n\tpos(%f/%f/%f)\n\tyaw(%f);\n\tpitch(%f)\n", pos.x, pos.y, pos.z, yaw, pitch);
	}
	
	if( input()->isKeyDown(ciri::Key::F5) && input()->wasKeyUp(ciri::Key::F5) ) {
		_waterShader->destroy();
		loadShaders();
		printf("Reloaded shaders.\n");
	}


	if( input()->isMouseButtonDown(ciri::MouseButton::Right) ) {
		if( input()->isKeyDown(ciri::Key::W) ) {
			_camera.move(ciri::FPSCamera::Direction::Forward, deltaTime);
		}
		if( input()->isKeyDown(ciri::Key::S) ) {
			_camera.move(ciri::FPSCamera::Direction::Backward, deltaTime);
		}
		if( input()->isKeyDown(ciri::Key::A) ) {
			_camera.move(ciri::FPSCamera::Direction::Left, deltaTime);
		}
		if( input()->isKeyDown(ciri::Key::D) ) {
			_camera.move(ciri::FPSCamera::Direction::Right, deltaTime);
		}
		const float mdx = (float)input()->mouseX() - (float)input()->lastMouseX();
		const float mdy = (float)input()->mouseY() - (float)input()->lastMouseY();
		_camera.rotateYaw(mdx);
		_camera.rotatePitch(mdy);
	}

	// update shader constant for elapsed time
	_waterConstants.ElapsedTime = elapsedTime;


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
	device->setBlendState(_blendState);

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
}