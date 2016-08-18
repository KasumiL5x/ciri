#include "playground.hpp"
#include "../../common/ModelGen.hpp"
#include <cc/MatrixFunc.hpp>

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
	//_camera.setSensitivity(100.0f, 10.0f);
	_camera.setPosition(cc::Vec3f(-45.5f, 33.0f, 32.0f));
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
	_blendState = graphicsDevice()->getDefaultBlendOpaque();

	// hook up player controller
	_plane = std::make_shared<Plane>();
	_playerPlaneController = std::make_shared<PlayerPlaneController>(_plane, input());
	_plane->setController(_playerPlaneController);
	_plane->setPosition(cc::Vec3f(0.0f, 10.0f, 0.0));

	// load plane model
	_planeModel = new Model();
	if( !_planeModel->addFromObj("data/demos/playground/frigate.obj") ) {
		printf("Failed to load plane model.\n");
	}
	//_planeModel = modelgen::createSphere(graphicsDevice(), 10, 10.0f);//, 10.0f, 10.0f, 10.0f, 1.0f, 1.0f);
	if( !_planeModel->build(graphicsDevice()) ) {
		printf("Failed to build plane model.\n");
	}

	_renderTarget = graphicsDevice()->createRenderTarget2D(512, 512, ciri::TextureFormat::RGBA32_Float, ciri::DepthStencilFormat::Depth24);
	ciri::DepthStencilDesc depthDesc;
	depthDesc.depthFunc = ciri::CompareFunction::Less;
	depthDesc.depthWriteMask = true;
	depthDesc.depthEnable = true;
	//depthDesc.stencilEnable = false;
	//depthDesc.twoSidedStencil = false;
	//depthDesc.stencilWriteMask = 1;
	//depthDesc.frontStencilFunc = ciri::CompareFunction::Always;
	//depthDesc.stencilRef = 1;
	//depthDesc.stencilReadMask = 0;
	//depthDesc.frontStencilFailOp = ciri::StencilOperation::Keep;
	//depthDesc.frontStencilDepthFailOp = ciri::StencilOperation::Replace;
	//depthDesc.frontStencilPassOp = ciri::StencilOperation::Replace;
	//depthDesc.frontStencilFailOp = depthDesc.frontStencilDepthFailOp = depthDesc.frontStencilPassOp = ciri::StencilOperation::Replace;

	_depthStencilState = graphicsDevice()->createDepthStencilState(depthDesc);
	_spriteBatch = std::make_shared<ciri::SpriteBatch>();
	_spriteBatch->create(graphicsDevice());
	_samplerState = graphicsDevice()->createSamplerState(ciri::SamplerDesc());
	ciri::SamplerDesc shadowSampler;
	shadowSampler.filter = ciri::SamplerFilter::Point;
	shadowSampler.wrapU = shadowSampler.wrapV = shadowSampler.wrapW = ciri::SamplerWrap::Border;
	shadowSampler.borderColor[0] = shadowSampler.borderColor[1] = shadowSampler.borderColor[2] = shadowSampler.borderColor[3] = 1.0f;
	_shadowSampler = graphicsDevice()->createSamplerState(shadowSampler);

	_groundModel = modelgen::createPlane(graphicsDevice(), 100.0f, 100.0f);
	_groundModel->getXform().setPosition(cc::Vec3f(50.0f, -10.0f, 50.0f));

	_spriteRaster = graphicsDevice()->getDefaultRasterNone();
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
		_basicShader->destroy();
		_depthShader->destroy();
		loadShaders();
		printf("Reloaded shaders.\n");
	}

	// update shader constant for elapsed time
	_waterConstants.ElapsedTime = elapsedTime;

	// update player controller
	_playerPlaneController->update(deltaTime, elapsedTime);

	// update camera stuff
	_cameraOrientation = cc::Quatf::lerp(_cameraOrientation, _plane->orientation(), 0.05f);
	const cc::Mat4f cameraOrientMatrix = cc::Quatf::createMatrixFromQuaternion(_cameraOrientation);
	cc::Vec3f camPos(0.0f, 10.0f, 175.f);
	camPos = (cameraOrientMatrix*cc::Vec4f(camPos, 1.0f)).truncated();
	camPos += _plane->position();
	cc::Vec3f camUp(0.0f, 1.0f, 0.0f);
	//camUp = (cameraOrientMatrix*cc::Vec4f(camUp, 1.0f)).truncated();
	_camera.setPosition(camPos);
	_camera.setUp(camUp);
	_camera.setTarget(_plane->position());
	//if( input()->isKeyDown(ciri::Key::I) ) {
	//	_camera.move(ciri::FPSCamera::Direction::Forward, deltaTime);
	//}
	//if( input()->isKeyDown(ciri::Key::K) ) {
	//	_camera.move(ciri::FPSCamera::Direction::Backward, deltaTime);
	//}
	//if( input()->isKeyDown(ciri::Key::J) ) {
	//	_camera.move(ciri::FPSCamera::Direction::Left, deltaTime);
	//}
	//if( input()->isKeyDown(ciri::Key::L) ) {
	//	_camera.move(ciri::FPSCamera::Direction::Right, deltaTime);
	//}
	//if( input()->isMouseButtonDown(ciri::MouseButton::Right) ) {
	//	_camera.rotatePitch((input()->lastMouseY() - input()->mouseY()) * deltaTime);
	//	_camera.rotateYaw(-(input()->lastMouseX() - input()->mouseX()) * deltaTime);
	//}

	if( input()->isKeyDown(ciri::Key::F11) && input()->wasKeyUp(ciri::Key::F11) ) {
		printf("Writing textures...");
		_renderTarget->getTexture()->writeToTGA("C:\\Users\\daniel\\Desktop\\test.tga");
		printf("done!\n");
	}
}

void Playground::onFixedUpdate(const double deltaTime, const double elapsedTime) {
	App::onFixedUpdate(deltaTime, elapsedTime);
}

void Playground::onDraw() {
	App::onDraw();

	const auto device = graphicsDevice();

	device->setRasterizerState(_rasterState);
	device->setDepthStencilState(_depthStencilState);

	////
	//// render depth from light first
	////
	device->applyShader(_depthShader);
	device->setViewport(ciri::Viewport(0, 0, _renderTarget->getTexture()->getWidth(), _renderTarget->getTexture()->getHeight()));
	const cc::Vec3f LIGHTPOS = cc::Vec3f(-10.0f, 40.0f, -10.0f);
	const cc::Vec3f LIGHTTAR = cc::Vec3f(20.0f, 0.0f, 20.0f);
	const cc::Vec3f LIGHTDIR = (LIGHTTAR - LIGHTPOS).normalized();
	const float LightOuterAngle = 35.0f;
	const float LightRange = 150.0f;
	//const cc::Mat4f lightView = cc::math::lookAtRH(LIGHTPOS, LIGHTPOS + LIGHTDIR, cc::Vec3f(0.0f, 1.0f, 0.0f));//cc::math::lookAtRH(LIGHTPOS, LIGHTPOS + LIGHTDIR, cc::Vec3f::up());
	//const cc::Mat4f lightProj = cc::math::perspectiveRH(LightOuterAngle * 2.0f, 1.0f, 1.0f, LightRange);
	const cc::Mat4f lightProj = cc::math::perspectiveRH(45.0f, 1.0f, 0.1f, LightRange);//cc::math::orthographic<float>(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, LightRange);
	const cc::Mat4f lightView = cc::math::lookAtRH(LIGHTPOS, LIGHTPOS + LIGHTDIR, cc::Vec3f(0.0f, 1.0f, 0.0f));
	//const cc::Mat4f lightViewProj = cc::Mat4f(-8.0f, -7.0f, -6.0f, -5.0f, -4.0f, -3.0f, -2.0f, -1.0f, 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f);//lightProj * lightView;
	const cc::Mat4f lightViewProj = lightProj * lightView;
	ciri::IRenderTarget2D* lightTargets = _renderTarget.get();
	device->setRenderTargets(&lightTargets, 1);
	device->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);
	// plane
	_depthConstants.xform = lightViewProj * _plane->world();
	_depthConstantBuffer->setData(sizeof(PGDepthConstants), &_depthConstants);
	device->setVertexBuffer(_planeModel->getVertexBuffer());
	device->drawArrays(ciri::PrimitiveTopology::TriangleList, _planeModel->getVertexBuffer()->getVertexCount(), 0);
	// ground
	_depthConstants.xform = lightViewProj * cc::Mat4f(1.0f);
	_depthConstantBuffer->setData(sizeof(PGDepthConstants), &_depthConstants);
	device->setVertexBuffer(_groundModel->getVertexBuffer());
	device->setIndexBuffer(_groundModel->getIndexBuffer());
	device->drawIndexed(ciri::PrimitiveTopology::TriangleList, _groundModel->getIndexBuffer()->getIndexCount());
	// restore targets and vp
	device->restoreDefaultRenderTargets();
	device->setViewport(ciri::Viewport(0, 0, window()->getWidth(), window()->getHeight()));

	const cc::Mat4f cameraViewProj = _camera.getProj() * _camera.getView();
	device->setClearColor(1.0f, 0.0f, 1.0f, 1.0f);
	device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);

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

	if( _planeModel->isValid() && _basicShader->isValid() ) {
		// update constants
		_basicConstants.world = _plane->world();
		_basicConstants.xform = cameraViewProj * _basicConstants.world;
		_basicConstants.campos = _camera.getPosition();
		_basicConstants.lightViewProj = lightViewProj;
		if( ciri::failed(_basicConstantBuffer->setData(sizeof(BasicConstants), &_basicConstants)) ) {
			printf("Failed to update basic constants.\n");
		} else {
			device->applyShader(_basicShader);
			device->setVertexBuffer(_planeModel->getVertexBuffer());
			device->setTexture2D(0, _renderTarget->getDepth(), ciri::ShaderStage::Pixel);
			device->setSamplerState(0, _shadowSampler, ciri::ShaderStage::Pixel);
			if( _planeModel->getIndexBuffer() != nullptr ) {
				device->setIndexBuffer(_planeModel->getIndexBuffer());
				device->drawIndexed(ciri::PrimitiveTopology::TriangleList, _planeModel->getIndexBuffer()->getIndexCount());
			} else {
				device->drawArrays(ciri::PrimitiveTopology::TriangleList, _planeModel->getVertexBuffer()->getVertexCount(), 0);
			}
		}
	}
	_basicConstants.world = _groundModel->getXform().getWorld();
	_basicConstants.xform = cameraViewProj * _basicConstants.world;
	_basicConstants.campos = _camera.getPosition();
	_basicConstants.lightViewProj = lightViewProj;
	_basicConstantBuffer->setData(sizeof(BasicConstants), &_basicConstants);
	device->applyShader(_basicShader);
	device->setTexture2D(0, _renderTarget->getDepth(), ciri::ShaderStage::Pixel);
	device->setSamplerState(0, _shadowSampler, ciri::ShaderStage::Pixel);
	device->setVertexBuffer(_groundModel->getVertexBuffer());
	device->setIndexBuffer(_groundModel->getIndexBuffer());
	device->drawIndexed(ciri::PrimitiveTopology::TriangleList, _groundModel->getIndexBuffer()->getIndexCount());

	//// render to texture
	//ciri::IRenderTarget2D* renderTarget = _renderTarget.get();
	//ciri::Viewport rtvp = ciri::Viewport(0, 0, _renderTarget->getTexture()->getWidth(), _renderTarget->getTexture()->getHeight(), 0.0f, 1.0f);
	//device->setViewport(rtvp);
	//device->setRenderTargets(&renderTarget, 1);
	//device->setClearColor(1.0f, 0.0f, 1.0f, 1.0f);
	//device->setClearDepth(1.0f);
	//device->setClearStencil(0);
	//device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth | ciri::ClearFlags::Stencil);
	//// draw plane again
	//if( _planeModel->isValid() && _basicShader->isValid() ) {
	//	// update constants
	//	_basicConstants.world = _plane->world();
	//	_basicConstants.xform = cameraViewProj * _basicConstants.world;
	//	_basicConstants.campos = _camera.getPosition();
	//	if( ciri::failed(_basicConstantBuffer->setData(sizeof(BasicConstants), &_basicConstants)) ) {
	//		printf("Failed to update basic constants.\n");
	//	} else {
	//		device->applyShader(_basicShader);
	//		device->setVertexBuffer(_planeModel->getVertexBuffer());
	//		if( _planeModel->getIndexBuffer() != nullptr ) {
	//			device->setIndexBuffer(_planeModel->getIndexBuffer());
	//			device->drawIndexed(ciri::PrimitiveTopology::TriangleList, _planeModel->getIndexBuffer()->getIndexCount());
	//		} else {
	//			device->drawArrays(ciri::PrimitiveTopology::TriangleList, _planeModel->getVertexBuffer()->getVertexCount(), 0);
	//		}
	//	}
	//}
	//// restore old targets
	//device->restoreDefaultRenderTargets();

	// 1. write anything to depth color tex to see if it outputs.
	// 2. write depth to depth color tex and see if that works instead for shadows (don't forget to change sampler part)
	// 3. something...
	//.



	//// debug draw depth stuff
	//_spriteBatch->begin(_blendState, _samplerState, _depthStencilState, _spriteRaster, ciri::SpriteSortMode::Deferred, nullptr);
	//_spriteBatch->draw(_renderTarget->getTexture(), cc::Vec2f(0, 0), 0.0f, cc::Vec2f(0.0f, 0.0f), 1.0f, 0.0f);
	//_spriteBatch->end();

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

	//
	// depth shader
	//
	if( nullptr == _depthShader ) {
		_depthShader = graphicsDevice()->createShader();
		_depthShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Position, 0));
		_depthShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Normal, 0));
		_depthShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float4, ciri::VertexUsage::Tangent, 0));
		_depthShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float2, ciri::VertexUsage::Texcoord, 0));
	}
	const std::string depthVS = "data/demos/playground/depth_vs" + shaderExt;
	const std::string depthPS = "data/demos/playground/depth_ps" + shaderExt;
	if( ciri::failed(_depthShader->loadFromFile(depthVS.c_str(), nullptr, depthPS.c_str())) ) {
		printf("Failed to load depth shader:\n");
		for( auto& err : _depthShader->getErrors() ) {
			printf("%s\n", err.msg.c_str());
		}
	}
	if( nullptr == _depthConstantBuffer) {
		_depthConstantBuffer = graphicsDevice()->createConstantBuffer();
		if( ciri::failed(_depthConstantBuffer->setData(sizeof(PGDepthConstants), &_depthConstants)) ) {
			printf("Failed to set depth constants.\n");
		}
	}
	if( ciri::failed(_depthShader->addConstants(_depthConstantBuffer, "DepthConstants", ciri::ShaderStage::Vertex)) ) {
		printf("Failed to assign depth constants to shader.\n");
	}
}