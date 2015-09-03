#include "TerrainDemo.hpp"
#include "../../common/ModelGen.hpp"
#include <ciri/util/TGA.hpp>

TerrainDemo::TerrainDemo()
	: IDemo(), _depthStencilState(nullptr), _rasterizerState(nullptr), _waterPlane(nullptr), _waterShader(nullptr), _waterConstantsBuffer(nullptr),
		_alphaBlendState(nullptr), WATER_HEIGHT(10.0f), _cubemap(nullptr) {
}

TerrainDemo::~TerrainDemo() {
}

DemoConfig TerrainDemo::getConfig() {
	DemoConfig cfg;
	cfg.windowTitle = "ciri : Terrain Demo";
	cfg.windowWidth = 1280;
	cfg.windowHeight = 720;
	cfg.deviceType = ciri::GraphicsDeviceFactory::DirectX;
	return cfg;
}

void TerrainDemo::onInitialize() {
	// window size
	const cc::Vec2ui windowSize = window()->getSize();

	// configure camera
	_camera.setAspect(static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y));
	_camera.setPlanes(0.1f, 10000.0f);
	_camera.setYaw(246.0f);
	_camera.setPitch(21.4f);
	_camera.setSensitivity(100.0f, 1.0f);
	_camera.setPosition(cc::Vec3f(451.15f, 83.26f, -316.42f));
}

void TerrainDemo::onLoadContent() {
	// create depth stencil state
	ciri::DepthStencilDesc depthDesc;
	_depthStencilState = graphicsDevice()->createDepthStencilState(depthDesc);

	// cerate rasterizer state
	ciri::RasterizerDesc rasterDesc;
	rasterDesc.cullMode = ciri::CullMode::Clockwise;
	//rasterDesc.fillMode = ciri::FillMode::Wireframe;
	_rasterizerState = graphicsDevice()->createRasterizerState(rasterDesc);

	// load the simple shader
	_simpleShader.create(graphicsDevice());

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
	ciri::ITexture2D* grassTex = graphicsDevice()->createTexture2D(grassTga.getWidth(), grassTga.getHeight(), ciri::TextureFormat::Color, 0, grassTga.getPixels());
	ciri::ITexture2D* rockTex = graphicsDevice()->createTexture2D(rockTga.getWidth(), rockTga.getHeight(), ciri::TextureFormat::Color, 0, rockTga.getPixels());
	ciri::ITexture2D* sandTex = graphicsDevice()->createTexture2D(sandTga.getWidth(), sandTga.getHeight(), ciri::TextureFormat::Color, 0, sandTga.getPixels());
	ciri::ITexture2D* snowTex = graphicsDevice()->createTexture2D(snowTga.getWidth(), snowTga.getHeight(), ciri::TextureFormat::Color, 0, snowTga.getPixels());
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
		if( ciri::err::failed(_waterShader->loadFromFile(vsFile.c_str(), nullptr, psFile.c_str())) ) {
			printf("Failed to load water shader:\n");
			for( auto err : _waterShader->getErrors() ) {
				printf("%s\n", err.msg.c_str());
			}
		} else {
			_waterConstantsBuffer = graphicsDevice()->createConstantBuffer();
			if( ciri::err::failed(_waterConstantsBuffer->setData(sizeof(WaterConstants), &_waterConstants)) ) {
				printf("Failed to create water constants.\n");
			} else {
				if( ciri::err::failed(_waterShader->addConstants(_waterConstantsBuffer, "WaterConstants", ciri::ShaderStage::Vertex)) ) {
					printf("Failed to assign constants to water shader.\n");
				}
			}
		}
	}

	// create water plane to match the heightmap size
	_waterPlane = modelgen::createPlane(graphicsDevice(), float(heightmap.getWidth()), float(heightmap.getHeight()), 0, 0, 10.0f, 10.0f, false, false);
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

	/// commented out for now, because setting simply throws.
	/// p.s. at some point i need to make all constructors private so users can't just create their own instances
	/// and are instead forces to use the device to initially create the resource.  eventually...
	// load the cubemap
	//ciri::TGA cubeRight; cubeRight.loadFromFile("terrain/skybox/posx.tga", true);
	//ciri::TGA cubeLeft; cubeLeft.loadFromFile("terrain/skybox/negx.tga", true);
	//ciri::TGA cubeTop; cubeTop.loadFromFile("terrain/skybox/posy.tga", true);
	//ciri::TGA cubeBottom; cubeBottom.loadFromFile("terrain/skybox/negy.tga", true);
	//ciri::TGA cubeBack; cubeBack.loadFromFile("terrain/skybox/posz.tga", true);
	//ciri::TGA cubeFront; cubeFront.loadFromFile("terrain/skybox/negz.tga", true);
	//_cubemap = graphicsDevice()->createTextureCube(1024, 1024, cubeRight.getPixels(), cubeLeft.getPixels(), cubeTop.getPixels(), cubeBottom.getPixels(), cubeBack.getPixels(), cubeFront.getPixels());
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
	// get current input states
	ciri::KeyboardState currKeyState;
	ciri::MouseState currMouseState;
	ciri::Input::getKeyboardState(&currKeyState);
	ciri::Input::getMouseState(&currMouseState, window());

	// close w/ escape
	if( currKeyState.isKeyDown(ciri::Keyboard::Escape) ) {
		this->gtfo();
		return;
	}

	// debug camera info
	if( currKeyState.isKeyDown(ciri::Keyboard::F9) && _prevKeyState.isKeyUp(ciri::Keyboard::F9) ) {
		const cc::Vec3f& pos = _camera.getPosition();
		const float yaw = _camera.getYaw();
		const float pitch = _camera.getPitch();
		printf("Camera Information\n\tpos(%f/%f/%f)\n\tyaw(%f);\n\tpitch(%f)\n", pos.x, pos.y, pos.z, yaw, pitch);
	}

	// camera rotation
	if( currMouseState.isButtonDown(ciri::MouseButton::Right) ) {
			const float dx = (float)currMouseState.x - (float)_prevMouseState.x;
			const float dy = (float)currMouseState.y - (float)_prevMouseState.y;
			_camera.rotateYaw(dx);
			_camera.rotatePitch(-dy);
	}

	// real shitty camera speed modifier
	_camera.setMoveSpeed(currKeyState.isKeyDown(ciri::Keyboard::LShift) ? 500.0f : 100.0f);

	// camera movement
	cc::Vec3f cameraMovement;
	if( currKeyState.isKeyDown(ciri::Keyboard::W) ) {
		cameraMovement.z -= 1.0f;
	}
	if( currKeyState.isKeyDown(ciri::Keyboard::S) ) {
		cameraMovement.z += 1.0f;
	}
	if( currKeyState.isKeyDown(ciri::Keyboard::A) ) {
		cameraMovement.x -= 1.0f;
	}
	if( currKeyState.isKeyDown(ciri::Keyboard::D) ) {
		cameraMovement.x += 1.0f;
	}
	cameraMovement.normalize();
	_camera.move(cameraMovement * (float)deltaTime);

	//// todo: update simulation things here
	//std::vector<Vertex>& planeVertices = _plane->getVertices();
	//for( unsigned int i = 0; i < planeVertices.size(); ++i ) {
	//	const float x = sinf(elapsedTime + planeVertices[i].position.x) * 0.5f;
	//	const float z = sinf(elapsedTime + planeVertices[i].position.z) * 0.5f;
	//	planeVertices[i].position.y = x * z;
	//}
	//_plane->updateBuffers(true, false);

	// update previous input states
	_prevKeyState = currKeyState;
	_prevMouseState = currMouseState;
}

void TerrainDemo::onDraw() {
	ciri::IGraphicsDevice* device = graphicsDevice();
	
	device->setDepthStencilState(_depthStencilState);
	device->setRasterizerState(_rasterizerState);

	device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);

	const cc::Mat4f viewProj = _camera.getProj() * _camera.getView();

	// render axis
	if( _axis.isValid() ) {
		_axis.updateConstants(viewProj);
		device->applyShader(_axis.getShader());
		device->setVertexBuffer(_axis.getVertexBuffer());
		device->drawArrays(ciri::PrimitiveTopology::LineList, _axis.getVertexBuffer()->getVertexCount(), 0);
	}

	_terrain.draw(viewProj, graphicsDevice());

	// render water plane
	if( _waterPlane && _waterPlane->getShader() != nullptr && _waterPlane->getShader()->isValid() && _waterPlane->isValid() ) {
		// update constant buffers
		_waterConstants.world = _waterPlane->getXform().getWorld();
		_waterConstants.worldview = _camera.getView() * _waterConstants.world;
		_waterConstants.xform = viewProj * _waterConstants.world;
		_waterConstants.campos = _camera.getPosition();
		_waterConstantsBuffer->setData(sizeof(WaterConstants), &_waterConstants);

		// set water sampler and normal texture
		device->setSamplerState(0, _waterSampler, ciri::ShaderStage::Pixel);
		device->setTexture2D(0, _waterNormalMap, ciri::ShaderStage::Pixel);

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
	}

	device->present();
}

void TerrainDemo::onUnloadContent() {
	_terrain.clean();

	if( _waterPlane != nullptr ) {
		delete _waterPlane;
		_waterPlane = nullptr;
	}
}
