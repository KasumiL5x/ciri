#include "ShadowsDemo.hpp"
#include <cc/MatrixFunc.hpp>
#include "BoundingFrustum.hpp"
#include "BoundingBox.hpp"

ShadowsDemo::ShadowsDemo()
	: App(), _animateObjects(true) {
}

ShadowsDemo::~ShadowsDemo() {
}

void ShadowsDemo::onInitialize() {
	App::onInitialize();

	printf("Device: %s\n", graphicsDevice()->getGpuName());
	printf("API: %s\n", graphicsDevice()->getApiInfo());

	_camera.setAspect((float)window()->getWidth() / (float)window()->getHeight());
	_camera.setPlanes(1.0f, 100.0f);
	_camera.setSensitivity(100.0f, 10.0f);
}

void ShadowsDemo::onLoadContent() {
	App::onLoadContent();

	// load shaders
	loadShaders();

	// load states
	ciri::RasterizerDesc rasterDesc;
	rasterDesc.cullMode = ciri::CullMode::Clockwise;
	//rasterDesc.fillMode = ciri::FillMode::Wireframe;
	_rasterState = graphicsDevice()->createRasterizerState(rasterDesc);
	_additiveBlendState = graphicsDevice()->getDefaultBlendAdditive();

	// create shadow map stuff
	_shadowTarget = graphicsDevice()->createRenderTarget2D(2048, 2048, ciri::TextureFormat::RGBA32_Float, ciri::DepthStencilFormat::Depth24);
	ciri::SamplerDesc shadowSamplerDesc;
	shadowSamplerDesc.filter = ciri::SamplerFilter::Point;
	shadowSamplerDesc.wrapU=shadowSamplerDesc.wrapV=shadowSamplerDesc.wrapW = ciri::SamplerWrap::Border;
	shadowSamplerDesc.borderColor[0]=shadowSamplerDesc.borderColor[1]=shadowSamplerDesc.borderColor[2]=shadowSamplerDesc.borderColor[3] = 1.0f;
	_shadowSampler = graphicsDevice()->createSamplerState(shadowSamplerDesc);

	// load some models
	_ground = std::make_shared<Model>();
	_ground->addFromObj("data/demos/shadows/ground.obj");
	_ground->build(graphicsDevice());
	if(_ground->isValid()){_models.push_back(_ground);}
	_helicopterBody = std::make_shared<Model>();
	_helicopterBody->addFromObj("data/demos/shadows/helicopter_body.obj");
	_helicopterBody->build(graphicsDevice());
	if(_helicopterBody->isValid()){_models.push_back(_helicopterBody);}
	_helicopterBlades = std::make_shared<Model>();
	_helicopterBlades->addFromObj("data/demos/shadows/helicopter_blades.obj");
	_helicopterBlades->build(graphicsDevice());
	if(_helicopterBlades->isValid()){_models.push_back(_helicopterBlades);}
	_helicopterTail = std::make_shared<Model>();
	_helicopterTail->addFromObj("data/demos/shadows/helicopter_tail.obj");
	_helicopterTail->build(graphicsDevice());
	if(_helicopterTail->isValid()){_models.push_back(_helicopterTail);}

	// add some lights
	Light light0(Light::Type::Directional);
	light0.setDirection(cc::Vec3f(0.75f, -0.8f, 0.72f));
	light0.setCastShadows(true);
	_lights.push_back(light0);
	Light light1(Light::Type::Spot);
	light1.setPosition(cc::Vec3f(0.0f, 20.0f, 0.0f));
	light1.setRange(200.0f);
	light1.setConeInnerAngle(10.0f);
	light1.setConeOuterAngle(12.0f);
	light1.setCastShadows(true);
	light1.setDiffuseIntensity(1.0f);//0.25f);
	//_lights.push_back(light1);
	_cameraLight = &_lights.back();
}

void ShadowsDemo::onEvent(const ciri::WindowEvent& evt) {
	App::onEvent(evt);
}

void ShadowsDemo::onUpdate(const double deltaTime, const double elapsedTime) {
	App::onUpdate(deltaTime, elapsedTime);

	if( !window()->hasFocus() ) {
		return;
	}

	if( input()->isKeyDown(ciri::Key::F5) && input()->wasKeyUp(ciri::Key::F5) ) {
		_spotlightShader->destroy();
		_directionalShader->destroy();
		_depthShader->destroy();
		printf("Reloading shaders...");
		loadShaders();
		printf("done\n");
	}

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
	if( input()->isMouseButtonDown(ciri::MouseButton::Right) ) {
		_camera.rotatePitch((input()->lastMouseY() - input()->mouseY()) * deltaTime);
		_camera.rotateYaw(-(input()->lastMouseX() - input()->mouseX()) * deltaTime);
	}

	if( input()->isKeyDown(ciri::Key::P) && input()->wasKeyUp(ciri::Key::P) ) {
		_animateObjects = !_animateObjects;
	}
	if( _animateObjects ) {
		_helicopterBlades->getXform().setOrientation(_helicopterBlades->getXform().getOrientation() * cc::Quatf::createFromEulerAngles(0.0f, 750.0f*deltaTime, 0.0f));
		_helicopterTail->getXform().setOrientation(_helicopterTail->getXform().getOrientation() * cc::Quatf::createFromEulerAngles(-1500.0f*deltaTime, 0.0f, 0.0f));

		const float DIST = 1.0f;
		const float height = 8.0f + (sinf(elapsedTime * 1.0f) * DIST);
		_helicopterBody->getXform().setPosition(cc::Vec3f(0.0f, height, 0.0f));
		_helicopterBlades->getXform().setPosition(_helicopterBody->getXform().getPosition() + cc::Vec3f(0.0f, 3.654f, 4.979f));
		_helicopterTail->getXform().setPosition(_helicopterBody->getXform().getPosition() + cc::Vec3f(1.807f, 2.968f, -21.251f));
	}

	if( input()->isKeyDown(ciri::Key::Space) && input()->wasKeyUp(ciri::Key::Space) ) {
		_lightFollowCamera = !_lightFollowCamera;
	}
	if( _lightFollowCamera ) {
		_cameraLight->setPosition(_camera.getPosition());
		_cameraLight->setDirection(_camera.getFpsFront());
	}

	if( input()->isKeyDown(ciri::Key::F11) && input()->wasKeyUp(ciri::Key::F11) ) {
		printf("Writing textures...");
		_shadowTarget->getDepth()->writeToTGA("C:\\Users\\daniel\\Desktop\\test.tga");
		printf("done!\n");
	}
}

void ShadowsDemo::onFixedUpdate(const double deltaTime, const double elapsedTime) {
	App::onUpdate(deltaTime, elapsedTime);
}

void ShadowsDemo::onDraw() {
	App::onDraw();

	const auto device = graphicsDevice();

	device->setClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);

	device->setRasterizerState(_rasterState);
	device->restoreDefaultBlendState();

	if( _spotlightShader->isValid() && _directionalShader->isValid() ) {
		const cc::Mat4f& cameraViewProj = _camera.getProj() * _camera.getView();

		bool firstLight = true;
		Light::Type boundLightType = Light::Type::Invalid;
		for( auto& light : _lights ) {
			// compute light matrices
			//const cc::Mat4f& lightView = light.view();
			//const cc::Mat4f& lightProj = light.proj();//cc::math::perspectiveRH(45.0f, 1.0f, 0.1f, light.range());//light.proj();
			//const cc::Mat4f lightViewProj = lightProj * lightView;
			if( light.type() == Light::Type::Directional ) {
				//light.computeViewProjFromFrustum(BoundingFrustum(cameraViewProj));
				light.computeViewProjFromFrustum(BoundingFrustum(_camera.getFov(), _camera.getAspect(), _camera.getNearPlane(), _camera.getFarPlane(), _camera.getPosition(), _camera.getFpsFront(), _camera.getUp()));
				//light.computeViewProjOrtho(_camera.getView(), _camera.getFov(), _camera.getAspect(), _camera.getNearPlane(), _camera.getFarPlane());
			}
			const cc::Mat4f lightViewProj = light.proj() * light.view();

			if( light.castShadows() ) {
				device->setDepthStencilState(device->getDefaultDepthStencilDefault());
				// set and clear render target
				ciri::IRenderTarget2D* depthTarget = _shadowTarget.get();
				device->setRenderTargets(&depthTarget, 1);
				device->setClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);
				// apply depth shader
				device->applyShader(_depthShader);
				// set viewport to depth size
				device->setViewport(ciri::Viewport(0, 0, _shadowTarget->getDepth()->getWidth(), _shadowTarget->getDepth()->getHeight()));
				// render all models
				for( auto& mdl : _models ) {
					_depthConstants.xform = lightViewProj * mdl->getXform().getWorld();
					_depthConstantsBuffer->setData(sizeof(DepthConstants), &_depthConstants);
					device->setVertexBuffer(mdl->getVertexBuffer());
					if( mdl->getIndexBuffer() != nullptr ) {
						device->setIndexBuffer(mdl->getIndexBuffer());
						device->drawIndexed(ciri::PrimitiveTopology::TriangleList, mdl->getIndexBuffer()->getIndexCount());
					} else {
						device->drawArrays(ciri::PrimitiveTopology::TriangleList, mdl->getVertexBuffer()->getVertexCount(), 0);
					}
				}

				// reser viewport to screen
				device->setViewport(ciri::Viewport(0, 0, window()->getWidth(), window()->getHeight()));
				// restore default render targets
				device->restoreDefaultRenderTargets();
			}
			switch( light.type() ) {
				case Light::Type::Directional: {
					if( boundLightType != Light::Type::Directional || light.castShadows() ) {
						boundLightType = Light::Type::Directional;
						device->applyShader(_directionalShader);
						device->setTexture2D(0, _shadowTarget->getDepth(), ciri::ShaderStage::Pixel);
						device->setSamplerState(0, _shadowSampler, ciri::ShaderStage::Pixel);
					}
					_directionalConstants.LightDirection = light.direction();
					_directionalConstants.LightColor = light.diffuseColor();
					_directionalConstants.LightIntensity = light.diffuseIntensity();
					_directionalConstants.campos = _camera.getPosition();
					_directionalConstants.CastShadows = light.castShadows();
					_directionalConstants.lightViewProj = lightViewProj;
					for( auto& mdl : _models ) {
						if( !mdl->isValid() ) {
							continue;
						}
						_directionalConstants.world = mdl->getXform().getWorld();
						_directionalConstants.xform = cameraViewProj * _directionalConstants.world;
						_directionalConstantsBuffer->setData(sizeof(DirectionalConstants), &_directionalConstants);
						device->setVertexBuffer(mdl->getVertexBuffer());
						if( mdl->getIndexBuffer() != nullptr ) {
							device->setIndexBuffer(mdl->getIndexBuffer());
							device->drawIndexed(ciri::PrimitiveTopology::TriangleList, mdl->getIndexBuffer()->getIndexCount());
						} else {
							device->drawArrays(ciri::PrimitiveTopology::TriangleList, mdl->getVertexBuffer()->getVertexCount(), 0);
						}
					}
					break;
				}
				case Light::Type::Spot: {
					if( boundLightType != Light::Type::Spot || light.castShadows() ) {
						boundLightType = Light::Type::Spot;
						device->applyShader(_spotlightShader);
						device->setTexture2D(0, _shadowTarget->getDepth(), ciri::ShaderStage::Pixel);
						device->setSamplerState(0, _shadowSampler, ciri::ShaderStage::Pixel);
					}
					_spotlightConstants.LightPosition = light.position();
					_spotlightConstants.LightDirection = light.direction();
					_spotlightConstants.LightColor = light.diffuseColor();
					_spotlightConstants.LightCosInner = light.cosConeInnerAngle(true);
					_spotlightConstants.LightCosOuter = light.cosConeOuterAngle(true);
					_spotlightConstants.LightIntensity = light.diffuseIntensity();
					_spotlightConstants.LightRange = light.range();
					_spotlightConstants.CastShadows = light.castShadows();
					_spotlightConstants.lightViewProj = lightViewProj;
					for( auto& mdl : _models ) {
						_spotlightConstants.world = mdl->getXform().getWorld();
						_spotlightConstants.xform = cameraViewProj * _spotlightConstants.world;
						_spotlightConstantsBuffer->setData(sizeof(SpotlightConstants), &_spotlightConstants);
						device->setVertexBuffer(mdl->getVertexBuffer());
						if( mdl->getIndexBuffer() != nullptr ) {
							device->setIndexBuffer(mdl->getIndexBuffer());
							device->drawIndexed(ciri::PrimitiveTopology::TriangleList, mdl->getIndexBuffer()->getIndexCount());
						} else {
							device->drawArrays(ciri::PrimitiveTopology::TriangleList, mdl->getVertexBuffer()->getVertexCount(), 0);
						}
					}
					break;
				}
			}

			if( firstLight ) {
				firstLight = false;
				device->setBlendState(_additiveBlendState);
			}
		}
	}

	device->present();
}

void ShadowsDemo::onUnloadContent() {
	App::onUnloadContent();
}

void ShadowsDemo::loadShaders() {
	const std::string shaderExt = graphicsDevice()->getShaderExt();

	//
	// spotlight
	//
	if( nullptr == _spotlightShader ) {
		_spotlightShader = graphicsDevice()->createShader();
		_spotlightShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Position, 0));
		_spotlightShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Normal, 0));
		_spotlightShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float4, ciri::VertexUsage::Tangent, 0));
		_spotlightShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float2, ciri::VertexUsage::Texcoord, 0));
	}
	const std::string spotVS = "data/demos/shadows/spot_vs" + shaderExt;
	const std::string spotPS = "data/demos/shadows/spot_ps" + shaderExt;
	if( ciri::failed(_spotlightShader->loadFromFile(spotVS.c_str(), nullptr, spotPS.c_str())) ) {
		printf("Failed to load spotlight shader:\n");
		std::for_each(_spotlightShader->getErrors().begin(), _spotlightShader->getErrors().end(), [](auto err){printf("%s\n", err.msg.c_str());});
	}
	if( nullptr == _spotlightConstantsBuffer ) {
		_spotlightConstantsBuffer = graphicsDevice()->createConstantBuffer();
	}
	if( ciri::failed(_spotlightConstantsBuffer->setData(sizeof(SpotlightConstants), &_spotlightConstants)) ) {
		printf("Failed to set spotlight constants.\n");
	} else {
		if( ciri::failed(_spotlightShader->addConstants(_spotlightConstantsBuffer, "SpotlightConstants", ciri::ShaderStage::Vertex)) ) {
			printf("Failed to assign spotlight constants.\n");
		}
	}

	//
	// directional
	//
	if( nullptr == _directionalShader ) {
		_directionalShader = graphicsDevice()->createShader();
		_directionalShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Position, 0));
		_directionalShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Normal, 0));
		_directionalShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float4, ciri::VertexUsage::Tangent, 0));
		_directionalShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float2, ciri::VertexUsage::Texcoord, 0));
	}
	const std::string dirVS = "data/demos/shadows/dir_vs" + shaderExt;
	const std::string dirPS = "data/demos/shadows/dir_ps" + shaderExt;
	if( ciri::failed(_directionalShader->loadFromFile(dirVS.c_str(), nullptr, dirPS.c_str())) ) {
		printf("Failed to load directional light shader:\n");
		std::for_each(_directionalShader->getErrors().begin(), _directionalShader->getErrors().end(), [](auto err){printf("%s\n", err.msg.c_str());});
	}
	if( nullptr == _directionalConstantsBuffer ) {
		_directionalConstantsBuffer = graphicsDevice()->createConstantBuffer();
	}
	if( ciri::failed(_directionalConstantsBuffer->setData(sizeof(DirectionalConstants), &_directionalConstants)) ) {
		printf("Failed to set directional light constants.\n");
	} else {
		if( ciri::failed(_directionalShader->addConstants(_directionalConstantsBuffer, "DirectionalConstants", ciri::ShaderStage::Vertex)) ) {
			printf("Failed to assign directional light constants.\n");
		}
	}

	//
	// depth
	//
	if( nullptr == _depthShader ) {
		_depthShader = graphicsDevice()->createShader();
		_depthShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Position, 0));
		_depthShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Normal, 0));
		_depthShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float4, ciri::VertexUsage::Tangent, 0));
		_depthShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float2, ciri::VertexUsage::Texcoord, 0));
	}
	const std::string depthVS = "data/demos/shadows/depth_vs" + shaderExt;
	const std::string depthPS = "data/demos/shadows/depth_ps" + shaderExt;
	if( ciri::failed(_depthShader->loadFromFile(depthVS.c_str(), nullptr, depthPS.c_str())) ) {
		printf("Failed to load depth shader.\n");
		std::for_each(_depthShader->getErrors().begin(), _depthShader->getErrors().end(), [](auto err){printf("%s\n", err.msg.c_str());});
	}
	if( nullptr == _depthConstantsBuffer ) {
		_depthConstantsBuffer = graphicsDevice()->createConstantBuffer();
	}
	if( ciri::failed(_depthConstantsBuffer->setData(sizeof(DepthConstants), &_depthConstants)) ) {
		printf("Failed to set depth constants.\n");
	} else {
		if( ciri::failed(_depthShader->addConstants(_depthConstantsBuffer, "DepthConstants", ciri::ShaderStage::Vertex)) ) {
			printf("Failed to assign depth constants.\n");
		}
	}
}