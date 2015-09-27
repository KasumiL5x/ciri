#include "TerrainDemo.hpp"
#include "../../common/ModelGen.hpp"
#include <ciri/core/TGA.hpp>
#include <ciri/core/PNG.hpp>

// fuck you microsoft
#undef far
#undef near
#include <cc/MatrixFunc.hpp>

TerrainDemo::TerrainDemo()
	: IDemo(), _depthStencilState(nullptr), _rasterizerState(nullptr), _waterPlane(nullptr), _waterShader(nullptr), _waterConstantsBuffer(nullptr),
		_alphaBlendState(nullptr), WATER_HEIGHT(10.0f), _waterReflectionTarget(nullptr), _cubemap(nullptr), _skyboxShader(nullptr), _skyboxConstantsBuffer(nullptr), _skyboxSampler(nullptr), _elapsedTime(0.0f) {
}

TerrainDemo::~TerrainDemo() {
}

DemoConfig TerrainDemo::getConfig() {
	DemoConfig cfg;
	cfg.windowTitle = "ciri : Terrain Demo";
	cfg.windowWidth = 1280;
	cfg.windowHeight = 720;
	return cfg;
}

void TerrainDemo::onInitialize() {
	// print info
	printf("Device: %s\n", graphicsDevice()->getGpuName());
	printf("API: %s\n", graphicsDevice()->getApiInfo());

	// configure camera
	_camera.setAspect(static_cast<float>(window()->getWidth()) / static_cast<float>(window()->getHeight()));
	_camera.setPlanes(0.1f, 10000.0f);
	_camera.setYaw(137.0f);
	_camera.setPitch(-12.6f);
	_camera.setSensitivity(100.0f, 1.0f);
	_camera.setPosition(cc::Vec3f(451.15f, 83.26f, -316.42f));
}

void TerrainDemo::onLoadContent() {
	//// create depth stencil state
	ciri::DepthStencilDesc depthDesc;
	_depthStencilState = graphicsDevice()->createDepthStencilState(depthDesc);

	// cerate rasterizer state
	ciri::RasterizerDesc rasterDesc;
	rasterDesc.cullMode = ciri::CullMode::Clockwise;
	//rasterDesc.fillMode = ciri::FillMode::Wireframe;
	_rasterizerState = graphicsDevice()->createRasterizerState(rasterDesc);

	// create axis
	_axis.create(5.0f, graphicsDevice());

	// create heightmap terrain
	ciri::TGA heightmap;
	heightmap.loadFromFile("terrain/heightmap.tga", false);
	if( !_terrain.generate(heightmap, graphicsDevice()) ) {
		printf("Failed to generate heightmap terrain.\n");
	}
	// load a bunch of terrain textures and set them
	ciri::TGA grassTga; grassTga.loadFromFile("terrain/grass.tga", true);
	ciri::TGA rockTga; rockTga.loadFromFile("terrain/rock.tga", true);
	ciri::TGA sandTga; sandTga.loadFromFile("terrain/sand.tga", true);
	ciri::TGA snowTga; snowTga.loadFromFile("terrain/snow.tga", true);
	std::shared_ptr<ciri::ITexture2D> grassTex = graphicsDevice()->createTexture2D(grassTga.getWidth(), grassTga.getHeight(), ciri::TextureFormat::Color, 0, grassTga.getPixels());
	std::shared_ptr<ciri::ITexture2D> rockTex = graphicsDevice()->createTexture2D(rockTga.getWidth(), rockTga.getHeight(), ciri::TextureFormat::Color, 0, rockTga.getPixels());
	std::shared_ptr<ciri::ITexture2D> sandTex = graphicsDevice()->createTexture2D(sandTga.getWidth(), sandTga.getHeight(), ciri::TextureFormat::Color, 0, sandTga.getPixels());
	std::shared_ptr<ciri::ITexture2D> snowTex = graphicsDevice()->createTexture2D(snowTga.getWidth(), snowTga.getHeight(), ciri::TextureFormat::Color, 0, snowTga.getPixels());
	_terrain.setTextures(grassTex, rockTex, sandTex, snowTex);

	// configure, load, etc, the water shader and its constants
	{
		_waterShader = graphicsDevice()->createShader();
		_waterShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Position, 0));
		_waterShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Normal, 0));
		_waterShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float4, ciri::VertexUsage::Tangent, 0));
		_waterShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float2, ciri::VertexUsage::Texcoord, 0));
		const std::string shaderExt = graphicsDevice()->getShaderExt();
		const std::string vsFile = ("terrain/water_vs" + shaderExt);
		const std::string psFile = ("terrain/water_ps" + shaderExt);
		if( ciri::failed(_waterShader->loadFromFile(vsFile.c_str(), nullptr, psFile.c_str())) ) {
			printf("Failed to load water shader:\n");
			for( auto err : _waterShader->getErrors() ) {
				printf("%s\n", err.msg.c_str());
			}
		} else {
			_waterConstantsBuffer = graphicsDevice()->createConstantBuffer();
			if( ciri::failed(_waterConstantsBuffer->setData(sizeof(WaterConstants), &_waterConstants)) ) {
				printf("Failed to create water constants.\n");
			} else {
				if( ciri::failed(_waterShader->addConstants(_waterConstantsBuffer, "WaterConstants", ciri::ShaderStage::Vertex)) ) {
					printf("Failed to assign constants to water shader.\n");
				}
			}
		}
	}

	// create water plane to match the heightmap size
	_waterPlane = modelgen::createPlane(graphicsDevice(), float(heightmap.getWidth()), float(heightmap.getHeight()), 100, 100, 10.0f, 10.0f, false, false);
	_waterPlane->setShader(_waterShader);
	// position the water up a little
	_waterPlane->getXform().setPosition(cc::Vec3f(0.0f, WATER_HEIGHT, 0.0f));
	// create water sampler and load water normal texture
	ciri::SamplerDesc samplerDesc;
	_waterSampler = graphicsDevice()->createSamplerState(samplerDesc);
	ciri::TGA waterNormals;
	waterNormals.loadFromFile("terrain/water_normals.tga", true);
	_waterNormalMap = graphicsDevice()->createTexture2D(waterNormals.getWidth(), waterNormals.getHeight(), ciri::TextureFormat::Color, 0, waterNormals.getPixels());

	// create alpha blend state
	ciri::BlendDesc alphaBlendDesc;
	alphaBlendDesc.colorFunc = ciri::BlendFunction::Add;
	alphaBlendDesc.alphaFunc = ciri::BlendFunction::Add;
	alphaBlendDesc.srcColorBlend = ciri::BlendMode::SourceAlpha;
	alphaBlendDesc.dstColorBlend = ciri::BlendMode::InverseSourceAlpha;
	alphaBlendDesc.srcAlphaBlend = ciri::BlendMode::One;
	alphaBlendDesc.dstAlphaBlend = ciri::BlendMode::Zero;
	_alphaBlendState = graphicsDevice()->createBlendState(alphaBlendDesc);

	// create render target for water reflections
	const unsigned int RTT_WIDTH = 640;
	const unsigned int RTT_HEIGHT = 360;
	_waterReflectionTarget = graphicsDevice()->createRenderTarget2D(RTT_WIDTH, RTT_HEIGHT, ciri::TextureFormat::RGBA32_Float);
// and for refractions
	_waterRefractionTarget = graphicsDevice()->createRenderTarget2D(RTT_WIDTH, RTT_HEIGHT, ciri::TextureFormat::RGBA32_Float);
	// and the sampler
	ciri::SamplerDesc reflSamplerDesc;
	reflSamplerDesc.filter = ciri::SamplerFilter::Linear;
	reflSamplerDesc.wrapU = ciri::SamplerWrap::Wrap;//Mirror; ?
	reflSamplerDesc.wrapV = ciri::SamplerWrap::Wrap;//Mirror;
	reflSamplerDesc.wrapW = ciri::SamplerWrap::Wrap;//Mirror;
	_waterReflectionSampler = graphicsDevice()->createSamplerState(reflSamplerDesc);

	// load the cubemap
	ciri::PNG cubeRight; cubeRight.loadFromFile("terrain/skybox/posx.png");
	ciri::PNG cubeLeft; cubeLeft.loadFromFile("terrain/skybox/negx.png");
	ciri::PNG cubeTop; cubeTop.loadFromFile("terrain/skybox/negy.png");
	ciri::PNG cubeBottom; cubeBottom.loadFromFile("terrain/skybox/posy.png");
	ciri::PNG cubeBack; cubeBack.loadFromFile("terrain/skybox/posz.png");
	ciri::PNG cubeFront; cubeFront.loadFromFile("terrain/skybox/negz.png");
	_cubemap = graphicsDevice()->createTextureCube(cubeRight.getWidth(), cubeRight.getHeight(), cubeRight.getPixels(), cubeLeft.getPixels(), cubeTop.getPixels(), cubeBottom.getPixels(), cubeBack.getPixels(), cubeFront.getPixels());
	// create the skybox model
	_skybox = modelgen::createFullscreenQuad(graphicsDevice());
	// load skybox shader
	{
		_skyboxShader = graphicsDevice()->createShader();
		_skyboxShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Position, 0));
		_skyboxShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Normal, 0));
		_skyboxShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float4, ciri::VertexUsage::Tangent, 0));
		_skyboxShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float2, ciri::VertexUsage::Texcoord, 0));
		const std::string shaderExt = graphicsDevice()->getShaderExt();
		const std::string vsFile = ("common/shaders/skybox_vs" + shaderExt);
		const std::string psFile = ("common/shaders/skybox_ps" + shaderExt);
		if( ciri::failed(_skyboxShader->loadFromFile(vsFile.c_str(), nullptr, psFile.c_str())) ) {
			printf("Failed to load skybox shader:\n");
			for( auto err : _skyboxShader->getErrors() ) {
				printf("%s\n", err.msg.c_str());
			}
		} else {
			_skyboxConstantsBuffer = graphicsDevice()->createConstantBuffer();
			if( ciri::failed(_skyboxConstantsBuffer->setData(sizeof(SkyboxConstants), &_skyboxConstants)) ) {
				printf("Failed to create skybox constants.\n");
			} else {
				if( ciri::failed(_skyboxShader->addConstants(_skyboxConstantsBuffer, "SkyboxConstants", ciri::ShaderStage::Vertex)) ) {
					printf("Failed to assign constants to skybox shader.\n");
				}
			}
		}
	}
	// skybox sampler
	ciri::SamplerDesc skySamplerDesc;
	skySamplerDesc.filter = ciri::SamplerFilter::Linear;
	skySamplerDesc.wrapU = ciri::SamplerWrap::Clamp;
	skySamplerDesc.wrapV = ciri::SamplerWrap::Clamp;
	skySamplerDesc.wrapW = ciri::SamplerWrap::Clamp;
	_skyboxSampler = graphicsDevice()->createSamplerState(skySamplerDesc);
	// skybox depth state
	ciri::DepthStencilDesc skyboxDepthDesc;
	skyboxDepthDesc.depthWriteMask = false;
	_skyboxDepthState = graphicsDevice()->createDepthStencilState(skyboxDepthDesc);
}

void TerrainDemo::onEvent( ciri::WindowEvent evt ) {
	switch( evt.type ) {
		case ciri::WindowEvent::Resized: {
			graphicsDevice()->resize();
			break;
		}
	}
}

void TerrainDemo::onUpdate( double deltaTime, double elapsedTime ) {
	_elapsedTime = static_cast<float>(elapsedTime);

	if( window()->hasFocus() ) {
		// close w/ escape
		if( input()->isKeyDown(ciri::Key::Escape) ) {
			this->gtfo();
			return;
		}

		// debug camera info
		if( input()->isKeyDown(ciri::Key::F9) && input()->wasKeyUp(ciri::Key::F9) ) {
			const cc::Vec3f& pos = _camera.getPosition();
			const float yaw = _camera.getYaw();
			const float pitch = _camera.getPitch();
			printf("Camera Information\n\tpos(%f/%f/%f)\n\tyaw(%f);\n\tpitch(%f)\n", pos.x, pos.y, pos.z, yaw, pitch);
		}

		// camera rotation
		if( input()->isMouseButtonDown(ciri::MouseButton::Right) || input()->isKeyDown(ciri::Key::LAlt) ) {
				const float dx = (float)input()->mouseX() - (float)input()->lastMouseX();
				const float dy = (float)input()->mouseY() - (float)input()->lastMouseY();
				_camera.rotateYaw(dx);
				_camera.rotatePitch(dy);
		}

		// real shitty camera speed modifier
		_camera.setMoveSpeed(input()->isKeyDown(ciri::Key::LShift) ? 500.0f : 100.0f);

		// camera movement
		if( input()->isKeyDown(ciri::Key::W) ) {
			_camera.move(ciri::FPSCamera::Direction::Forward, static_cast<float>(deltaTime));
		}
		if( input()->isKeyDown(ciri::Key::S) ) {
			_camera.move(ciri::FPSCamera::Direction::Backward, static_cast<float>(deltaTime));
		}
		if( input()->isKeyDown(ciri::Key::A) ) {
			_camera.move(ciri::FPSCamera::Direction::Left, static_cast<float>(deltaTime));
		}
		if( input()->isKeyDown(ciri::Key::D) ) {
			_camera.move(ciri::FPSCamera::Direction::Right, static_cast<float>(deltaTime));
		}

		// shader reloading
		if( input()->isKeyDown(ciri::Key::F5) && input()->wasKeyUp(ciri::Key::F5) ) {
			_waterShader->destroy();
			const std::string shaderExt = graphicsDevice()->getShaderExt();
			const std::string vsFile = ("terrain/water_vs" + shaderExt);
			const std::string psFile = ("terrain/water_ps" + shaderExt);
			if( ciri::failed(_waterShader->loadFromFile(vsFile.c_str(), nullptr, psFile.c_str())) ) {
				printf("Failed to load water shader:\n");
				for( auto err : _waterShader->getErrors() ) {
					printf("%s\n", err.msg.c_str());
				}
			} else {
				_waterShader->addConstants(_waterConstantsBuffer, "WaterConstants", ciri::ShaderStage::Vertex);
			}
		}

		// debug key
		if( input()->isKeyDown(ciri::Key::F11) && input()->wasKeyUp(ciri::Key::F11) ) {
			printf("debug...");
			_waterReflectionTarget->getTexture2D()->writeToDDS("C:/Users/kasum/Desktop/refl.dds");
			_waterRefractionTarget->getTexture2D()->writeToDDS("C:/Users/kasum/Desktop/refr.dds");
			printf("done\n");
		}
	}

	//// todo: update simulation things here
	//std::vector<Vertex>& planeVertices = _plane->getVertices();
	//for( unsigned int i = 0; i < planeVertices.size(); ++i ) {
	//	const float x = sinf(elapsedTime + planeVertices[i].position.x) * 0.5f;
	//	const float z = sinf(elapsedTime + planeVertices[i].position.z) * 0.5f;
	//	planeVertices[i].position.y = x * z;
	//}
	//_plane->updateBuffers(true, false);
}

void TerrainDemo::onFixedUpdate( double deltaTime, double elapsedTime ) {

}

void TerrainDemo::onDraw() {
	std::shared_ptr<ciri::IGraphicsDevice> device = graphicsDevice();

	// frame defaults
	device->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	device->setRasterizerState(_rasterizerState);
	device->setDepthStencilState(_depthStencilState);

	// render reflection map and refraction map
	{
		// attempt 1
		//const cc::Vec3f position = cc::Vec3f(_camera.getPosition().x, -_camera.getPosition().y + (WATER_HEIGHT * 2.0f), _camera.getPosition().z);
		//const cc::Vec3f flatForward = cc::Vec3f(_camera.getFpsFront().x, 0.0f, _camera.getFpsFront().z).normalized();
		//const cc::Mat4f viewMatrix = cc::math::lookAtRH(position, position + flatForward, cc::Vec3f::up());

		// attempt 2 (~works)
		const cc::Mat4f rotation = cc::math::rotate(_camera.getYaw(), cc::Vec3f(0.0f, 1.0f, 0.0f)) *
			cc::math::rotate(_camera.getPitch(), cc::Vec3f(1.0f, 0.0f, 0.0f));
		cc::Vec3f pos = _camera.getPosition();
		pos.y = -pos.y + (WATER_HEIGHT * 2.0f);
		const cc::Vec3f origTarget = _camera.getPosition() + _camera.getFpsFront();
		cc::Vec3f target = origTarget;
		target.y = -target.y + (WATER_HEIGHT * 2.0f);
		const cc::Vec3f right = (rotation * cc::Vec4f(1.0f, 0.0f, 0.0f, 1.0f)).truncated();
		const cc::Vec3f up = right.cross((target - pos).normalized()).normalized();
		const cc::Mat4f viewMatrix = cc::math::lookAtRH(pos, target, cc::Vec3f::up()); // if this is worldspace up, it works, otherwise, it tiles for some reason

		// camera matrices
		const cc::Mat4f projMatrix = _camera.getProj();
		const cc::Mat4f viewProjMatrix = projMatrix * viewMatrix;

		// reflections
		ciri::IRenderTarget2D* renderTarget = _waterReflectionTarget.get();
		device->setRenderTargets(&renderTarget, 1);
		device->clear(ciri::ClearFlags::Color);
		drawSkybox(viewMatrix, projMatrix);
		_terrain.setClippingPlaneActive(true);
		_terrain.setClippingPlaneParams(WATER_HEIGHT - 0.5f, viewProjMatrix, true);
		_terrain.draw(viewProjMatrix, device);
		_waterConstants.reflectedViewProj = viewProjMatrix;

		// refractions
		const cc::Mat4f standardViewProj = _camera.getProj() * _camera.getView();
		renderTarget = _waterRefractionTarget.get();
		device->setRenderTargets(&renderTarget, 1);
		device->clear(ciri::ClearFlags::Color);
		_terrain.setClippingPlaneActive(true);
		_terrain.setClippingPlaneParams(WATER_HEIGHT + 1.5f, standardViewProj, false);
		_terrain.draw(standardViewProj, device);
	}

	// set and clear default render target
	device->restoreDefaultRenderTargets();
	device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);

	// render skybox
	drawSkybox(_camera.getView(), _camera.getProj());

	const cc::Mat4f viewProj = _camera.getProj() * _camera.getView();

	// render axis
	if( _axis.isValid() ) {
		_axis.updateConstants(viewProj);
		device->applyShader(_axis.getShader());
		device->setVertexBuffer(_axis.getVertexBuffer());
		device->drawArrays(ciri::PrimitiveTopology::LineList, _axis.getVertexBuffer()->getVertexCount(), 0);
	}

	static bool enableTerrain = true;
	enableTerrain = input()->isKeyDown(ciri::Key::Space);
	if( !enableTerrain ) {
		_terrain.setClippingPlaneActive(false);
		_terrain.draw(viewProj, graphicsDevice());
	}

	// render water plane
	if( _waterPlane && _waterPlane->getShader() != nullptr && _waterPlane->getShader()->isValid() && _waterPlane->isValid() ) {
		// update constant buffers
		_waterConstants.world = _waterPlane->getXform().getWorld();
		_waterConstants.xform = viewProj * _waterConstants.world;
		_waterConstants.campos = _camera.getPosition();
		_waterConstants.time = _elapsedTime;
		_waterConstantsBuffer->setData(sizeof(WaterConstants), &_waterConstants);

		// set water sampler and normal texture
		device->setSamplerState(0, _waterSampler, ciri::ShaderStage::Pixel);
		device->setTexture2D(0, _waterNormalMap, ciri::ShaderStage::Pixel);

		// set reflection texture and sampler
		device->setSamplerState(1, _waterReflectionSampler, ciri::ShaderStage::Pixel);
		device->setTexture2D(1, _waterReflectionTarget->getTexture2D(), ciri::ShaderStage::Pixel);

		// set refraction texture and sampler
		device->setSamplerState(2, _waterReflectionSampler, ciri::ShaderStage::Pixel);
		device->setTexture2D(2, _waterRefractionTarget->getTexture2D(), ciri::ShaderStage::Pixel);

		// enable alpha blending
		device->setBlendState(_alphaBlendState);

		// apply shader
		device->applyShader(_waterPlane->getShader());

		// set buffers and draw
		device->setVertexBuffer(_waterPlane->getVertexBuffer());
		device->setIndexBuffer(_waterPlane->getIndexBuffer());
		device->drawIndexed(ciri::PrimitiveTopology::TriangleList, _waterPlane->getIndexBuffer()->getIndexCount());

		// restore default blend state
		device->setBlendState(nullptr);

		// reset sampler and texture slots
		device->setSamplerState(0, nullptr, ciri::ShaderStage::Pixel);
		device->setSamplerState(1, nullptr, ciri::ShaderStage::Pixel);
		device->setSamplerState(2, nullptr, ciri::ShaderStage::Pixel);
		device->setTexture2D(0, nullptr, ciri::ShaderStage::Pixel);
		device->setTexture2D(1, nullptr, ciri::ShaderStage::Pixel);
		device->setTexture2D(2, nullptr, ciri::ShaderStage::Pixel);
	}

	device->present();
}

void TerrainDemo::onUnloadContent() {
	_terrain.clean();

	if( _skybox != nullptr ) {
		delete _skybox;
		_skybox = nullptr;
	}

	if( _waterPlane != nullptr ) {
		delete _waterPlane;
		_waterPlane = nullptr;
	}
}


void TerrainDemo::drawSkybox( const cc::Mat4f& view, const cc::Mat4f& proj ) {
	if( !_skybox->isValid() ) {
		return;
	}

	// disable depth write
	graphicsDevice()->setDepthStencilState(_skyboxDepthState);
	// apply skybox shader
	graphicsDevice()->applyShader(_skyboxShader);
	// set skybox constants
	_skyboxConstants.view = view;
	_skyboxConstants.proj = proj;
	_skyboxConstants.proj.invert();
	_skyboxConstantsBuffer->setData(sizeof(SkyboxConstants), &_skyboxConstants);
	// set skybox texture and sampler
	graphicsDevice()->setTextureCube(0, _cubemap, ciri::ShaderStage::Pixel);
	graphicsDevice()->setSamplerState(0, _skyboxSampler, ciri::ShaderStage::Pixel);
	// set buffers and draw
	graphicsDevice()->setVertexBuffer(_skybox->getVertexBuffer());
	graphicsDevice()->setIndexBuffer(_skybox->getIndexBuffer());
	graphicsDevice()->drawIndexed(ciri::PrimitiveTopology::TriangleList, _skybox->getIndexBuffer()->getIndexCount());
	// unbind texture and sampler
	graphicsDevice()->setSamplerState(0, nullptr, ciri::ShaderStage::Pixel);
	graphicsDevice()->setTextureCube(0, nullptr, ciri::ShaderStage::Pixel);
	// restore default depth state
	graphicsDevice()->setDepthStencilState(_depthStencilState);
}