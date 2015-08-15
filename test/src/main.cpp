#include <vector>
#include <ciri/wnd/Window.hpp>
#include <ciri/gfx/GraphicsDeviceFactory.hpp>
#include <ciri/util/Timer.hpp>
#include <ciri/input/Input.hpp>
#include <ciri/gfx/MayaCamera.hpp>
#include <ciri/util/TGA.hpp>
#include <cc/Mat4.hpp>
#include "Model.hpp"
#include "ModelGen.hpp"
#include <ciri/gfx/ObjModel.hpp>
#include "Grid.hpp"
#include "Axis.hpp"
#include <ciri/util/Log.hpp>
#if defined(_DEBUG)
	#include <crtdbg.h>
#endif

__declspec(align(16))
struct SimpleConstants {
	cc::Mat4f world;
	cc::Mat4f xform;
};

const int SCR_W = 1280;
const int SCR_H = 720;
const ciri::GraphicsDeviceFactory::DeviceType GRAPHICS_DEVICE_TYPE = ciri::GraphicsDeviceFactory::OpenGL;
const std::string SHADER_EXT = (ciri::GraphicsDeviceFactory::OpenGL == GRAPHICS_DEVICE_TYPE) ? ".glsl" : ".hlsl";

void enableMemoryLeakChecking();
bool createWindow();
bool createGraphicsDevice();
bool createRasterizerStates();
bool createDepthStencilStates();
bool createRenderTargets();
bool loadShaders();
bool createConstantBuffers();
bool assignConstantBuffersToShaders();
bool loadModelFromObj( const char* file, Model* outModel );
bool loadModels();
bool loadTextures();
bool createSamplers();
void cleanup();

ciri::Window* window = nullptr;
ciri::IGraphicsDevice* graphicsDevice = nullptr;
ciri::IShader* simpleShader = nullptr;
bool areShadersValid;
SimpleConstants simpleConstants;
ciri::IConstantBuffer* simpleConstantsBuffer = nullptr;
std::vector<Model*> models;
Grid grid;
Axis axis;
ciri::ITexture2D* texture0 = nullptr;
ciri::ISamplerState* sampler0 = nullptr;
ciri::IRasterizerState* rasterizerState = nullptr;
ciri::IDepthStencilState* depthStencilState = nullptr;
ciri::IRenderTarget2D* renderTarget = nullptr;

int main() {
	enableMemoryLeakChecking();

	// start the debug log file
	ciri::Logs::get(ciri::Logs::Debug).open("debug.txt");
	ciri::Logs::get(ciri::Logs::Debug).printInfo("Debug log started!");

	// create the window
	if( !createWindow() ) {
		printf("Failed to initialize window.\n");
		cleanup();
		return -1;
	}

	// create the graphics device
	if( !createGraphicsDevice() ) {
		printf("Failed to create graphics device.\n");
		cleanup();
		return -1;
	}

	// create rasterizer states
	if( !createRasterizerStates() ) {
		printf("Failed to create rasterizer states.\n");
		cleanup();
		return -1;
	}

	// create depth stencil states
	if( !createDepthStencilStates() ) {
		printf("Failed to create depth stencil states.\n");
		cleanup();
		return -1;
	}

	// create render targets
	if( !createRenderTargets() ) {
		printf("Failed to create render targets.\n");
		cleanup();
		return -1;
	}

	// load shaders
	areShadersValid = loadShaders();
	if( !areShadersValid ) {
		printf("Failed to load shaders.\n");
		cleanup();
		return -1;
	}

	// create constant buffers
	if( !createConstantBuffers() ) {
		printf("Failed to create constant buffers.\n");
		cleanup();
		return -1;
	}

	// assign constant buffers to shaders
	if( !assignConstantBuffersToShaders() ) {
		printf("Failed to assign constant buffers to shaders.\n");
	}

	// load models
	if( !loadModels() ) {
		printf("Failed to load models.\n");
	}

	// load textures
	if( !loadTextures() ) {
		printf("Failed to load textures.\n");
	}

	// create samplers
	if( !createSamplers() ) {
		printf("Failed to create samplers.\n");
	}

	// create the world grid
	if( !grid.create(graphicsDevice, SHADER_EXT) ) {
		printf("Failed to create grid.\n");
	}

	// create the axis thingymabob
	if( !axis.create(5.0f, SHADER_EXT, graphicsDevice) ) {
		printf("Failed to create axis.\n");
	}

	// create and configure a maya-styled camera
	ciri::MayaCamera camera;
	camera.setAspect(static_cast<float>(SCR_W) / static_cast<float>(SCR_H));
	camera.setPlanes(0.1f, 1000.0f);
	camera.setYaw(0.0f);
	camera.setPitch(9.0f);
	camera.setOffset(4.0f);
	camera.setSensitivity(100.0f, 50.0f, 10.0f);
	camera.setLerpStrength(100.0f);
	camera.resetPosition();

	// create input states for mouse and keyboard
	ciri::KeyboardState currKeyState; ciri::Input::getKeyboardState(&currKeyState);
	ciri::KeyboardState prevKeyState = currKeyState;
	ciri::MouseState currMouseState; ciri::Input::getMouseState(&currMouseState, window);
	ciri::MouseState prevMouseState = currMouseState;
	
	// create a timer for measuring delta time between runs of the main loop
	double lastTime = 0.0;
	ciri::Timer timer;
	timer.start();

	// main loop
	while( window->isOpen() ) {
		// event processing
		ciri::WindowEvent evt;
		while( window->pollEvent(evt) ) {
			if( ciri::WindowEvent::Closed == evt.type ) {
				window->destroy();
				break;
			}
		}

		// delta time since last loop
		const double currTime = timer.getElapsedMicroseconds();
		const float deltaTime = static_cast<float>((currTime - lastTime) * 0.000001);
		lastTime = currTime;

		// get current input states
		ciri::Input::getKeyboardState(&currKeyState);
		ciri::Input::getMouseState(&currMouseState, window);

		// check for close
		if( currKeyState.isKeyDown(ciri::Keyboard::Escape) ) {
			window->destroy();
			break;
		}

		// camera movement
		if( currKeyState.isKeyDown(ciri::Keyboard::LAlt) ) {
			// rotation
			if( currMouseState.isButtonDown(ciri::MouseButton::Left) ) {
				const float dx = static_cast<float>(currMouseState.x) - static_cast<float>(prevMouseState.x);
				const float dy = static_cast<float>(currMouseState.y) - static_cast<float>(prevMouseState.y);
				camera.rotateYaw(-dx * deltaTime);
				camera.rotatePitch(-dy * deltaTime);
			}
			// dolly
			if( currMouseState.isButtonDown(ciri::MouseButton::Right) ) {
				const float dx = static_cast<float>(currMouseState.x) - static_cast<float>(prevMouseState.x);
				const float dy = static_cast<float>(currMouseState.y) - static_cast<float>(prevMouseState.y);
				const float val = (fabsf(dx) > fabsf(dy)) ? dx : dy;
				camera.dolly(val * deltaTime);
			}
			// pan
			if( currMouseState.isButtonDown(ciri::MouseButton::Middle) ) {
				const float dx = static_cast<float>(currMouseState.x) - static_cast<float>(prevMouseState.x);
				const float dy = static_cast<float>(currMouseState.y) - static_cast<float>(prevMouseState.y);
				camera.pan(dx * deltaTime, -dy * deltaTime);
			}
		}
		camera.update(deltaTime);

		const cc::Mat4f cameraViewProj = camera.getProj() * camera.getView();

		graphicsDevice->restoreDefaultRenderTargets();
		graphicsDevice->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);
		graphicsDevice->setDepthStencilState(depthStencilState);

		if( grid.isValid() ) {
			if( grid.updateConstants(cameraViewProj) ) {
				graphicsDevice->applyShader(grid.getShader());
				graphicsDevice->setVertexBuffer(grid.getVertexBuffer());
				graphicsDevice->drawArrays(ciri::PrimitiveTopology::LineList, grid.getVertexBuffer()->getVertexCount(), 0);
			}
		}

		if( axis.isValid() ) {
			if( axis.updateConstants(cameraViewProj) ) {
				graphicsDevice->applyShader(axis.getShader());
				graphicsDevice->setVertexBuffer(axis.getVertexBuffer());
				graphicsDevice->drawArrays(ciri::PrimitiveTopology::LineList, axis.getVertexBuffer()->getVertexCount(), 0);
			}
		}

		if( areShadersValid ) {
			graphicsDevice->setRasterizerState(rasterizerState);

			//
			// render the scene normally to texture
			//
			graphicsDevice->setRenderTargets(&renderTarget, 1);
			graphicsDevice->clear(ciri::ClearFlags::Color);// | ciri::ClearFlags::Depth);
			// render all models with the simple shader
			graphicsDevice->applyShader(simpleShader);
			graphicsDevice->setTexture2D(0, texture0, ciri::ShaderStage::Pixel);
			graphicsDevice->setSamplerState(0, sampler0, ciri::ShaderStage::Pixel);
			for( unsigned int i = 0; i < models.size(); ++i ) {
				Model* mdl = models[i];

				// update constant buffer for this object
				simpleConstants.world = mdl->getXform().getWorld();
				simpleConstants.xform = cameraViewProj * simpleConstants.world;
				if( ciri::err::failed(simpleConstantsBuffer->setData(sizeof(SimpleConstants), &simpleConstants)) ) {
					printf("Failed to update simple constants buffer.\n");
				}

				graphicsDevice->setVertexBuffer(mdl->getVertexBuffer());
				if( mdl->getIndexBuffer() != nullptr ) {
					graphicsDevice->setIndexBuffer(mdl->getIndexBuffer());
					graphicsDevice->drawIndexed(ciri::PrimitiveTopology::TriangleList, mdl->getIndexBuffer()->getIndexCount());
				} else {
					graphicsDevice->drawArrays(ciri::PrimitiveTopology::TriangleList, mdl->getVertexBuffer()->getVertexCount(), 0);
				}
			}
			graphicsDevice->setTexture2D(0, nullptr, ciri::ShaderStage::Pixel);
			graphicsDevice->setSamplerState(0, nullptr, ciri::ShaderStage::Pixel);

			//
			// render the scene using the rendertarget as a texture
			//
			graphicsDevice->restoreDefaultRenderTargets();
			// render all models with the simple shader
			graphicsDevice->applyShader(simpleShader);
			graphicsDevice->setTexture2D(0, renderTarget->getTexture2D(), ciri::ShaderStage::Pixel);
			graphicsDevice->setSamplerState(0, sampler0, ciri::ShaderStage::Pixel);
			for( unsigned int i = 0; i < models.size(); ++i ) {
				Model* mdl = models[i];

				// update constant buffer for this object
				simpleConstants.world = mdl->getXform().getWorld();
				simpleConstants.xform = cameraViewProj * simpleConstants.world;
				if( ciri::err::failed(simpleConstantsBuffer->setData(sizeof(SimpleConstants), &simpleConstants)) ) {
					printf("Failed to update simple constants buffer.\n");
				}

				graphicsDevice->setVertexBuffer(mdl->getVertexBuffer());
				if( mdl->getIndexBuffer() != nullptr ) {
					graphicsDevice->setIndexBuffer(mdl->getIndexBuffer());
					graphicsDevice->drawIndexed(ciri::PrimitiveTopology::TriangleList, mdl->getIndexBuffer()->getIndexCount());
				} else {
					graphicsDevice->drawArrays(ciri::PrimitiveTopology::TriangleList, mdl->getVertexBuffer()->getVertexCount(), 0);
				}
			}
			graphicsDevice->setTexture2D(0, nullptr, ciri::ShaderStage::Pixel);
			graphicsDevice->setSamplerState(0, nullptr, ciri::ShaderStage::Pixel);
		}

		graphicsDevice->present();

		if( currKeyState.isKeyDown(ciri::Keyboard::F2) && prevKeyState.isKeyUp(ciri::Keyboard::F2) ) {
			if( ciri::err::failed(renderTarget->getTexture2D()->writeToDDS("C:/Users/kasum/Desktop/rendertarget.dds")) ) {
				printf("Failed to write rendertarget to file.\n");
			} else {
				printf("Successfully written rendertarget to file.\n");
			}
		}

		// update previous input states
		prevKeyState = currKeyState;
		prevMouseState = currMouseState;
	}

	cleanup();

	return 0;
}

void enableMemoryLeakChecking() {
	#if defined(_DEBUG)
		int debugFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
		debugFlag|= _CRTDBG_LEAK_CHECK_DF;
		debugFlag |= _CRTDBG_CHECK_ALWAYS_DF;
		_CrtSetDbgFlag(debugFlag);
	#endif
}

bool createWindow() {
	window = new ciri::Window();
	return window->create(SCR_W, SCR_H);
}

bool createGraphicsDevice() {
	graphicsDevice = ciri::GraphicsDeviceFactory::create(GRAPHICS_DEVICE_TYPE);
	return graphicsDevice->create(window);
}

bool createRasterizerStates() {
	ciri::RasterizerDesc desc;
	desc.cullMode = ciri::CullMode::Clockwise;
	desc.msaa = true;
	//desc.fillMode = ciri::FillMode::Wireframe;
	rasterizerState = graphicsDevice->createRasterizerState(desc);
	return (rasterizerState != nullptr);
}

bool createDepthStencilStates() {
	ciri::DepthStencilDesc desc;
	desc.depthEnable = true;
	depthStencilState = graphicsDevice->createDepthStencilState(desc);
	return (depthStencilState != nullptr);
}

bool createRenderTargets() {
	renderTarget = graphicsDevice->createRenderTarget2D(SCR_W, SCR_H, ciri::TextureFormat::RGBA32_Float);
	return (renderTarget != nullptr);
}

bool loadShaders() {
	simpleShader = graphicsDevice->createShader();
	simpleShader->addVertexShader(("data/simple_vs" + SHADER_EXT).c_str());
	simpleShader->addPixelShader(("data/simple_ps" + SHADER_EXT).c_str());
	simpleShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Position, 0));
	simpleShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Normal, 0));
	simpleShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float2, ciri::VertexUsage::Texcoord, 0));
	if( ciri::err::failed(simpleShader->build()) ) {
		printf("Failed to compile simple shader: %s\n", simpleShader->getLastError());
		return false;
	}

	return true;
}

bool createConstantBuffers() {
	simpleConstantsBuffer = graphicsDevice->createConstantBuffer();
	if( ciri::err::failed(simpleConstantsBuffer->setData(sizeof(SimpleConstants), &simpleConstants)) ) {
		printf("Failed to create simple constant buffer.\n");
		return false;
	}

	return true;
}

bool assignConstantBuffersToShaders() {
	if( ciri::err::failed(simpleShader->addConstants(simpleConstantsBuffer, "SimpleConstants", ciri::ShaderStage::Vertex)) ) {
		printf("Failed to assign simple constants to simple shader.\n");
		return false;
	}

	return true;
}

bool loadModelFromObj( const char* file, Model* outModel ) {
	ciri::ObjModel obj;
	if( !obj.parse(file) ) {
		return false;
	}

	const std::vector<cc::Vec3f>& positions = obj.getPositions();
	const std::vector<cc::Vec3f>& normals = obj.getNormals();
	const std::vector<cc::Vec2f>& texcoords = obj.getTexcoords();
	const std::vector<ciri::ObjModel::ObjVertex>& vertices = obj.getVertices();
	for( unsigned int i = 0; i < vertices.size(); ++i ) {
		Vertex vert;
		vert.position = positions[vertices[i].position];
		vert.normal = normals[vertices[i].normal];
		vert.texcoord = texcoords[vertices[i].texcoord];
		outModel->addVertex(vert);
	}
	return outModel->build(graphicsDevice);
}

bool loadModels() {
	bool success = true;

	//Model* testModel = new Model();
	//if( !loadModelFromObj("data/lego.obj", testModel) ) {
	//	delete testModel;
	//	testModel = nullptr;
	//	success = false;
	//	printf("Failed to load test model.\n");
	//} else {
	//	models.push_back(testModel);
	//}

	// create a floor
	Model* floor = modelgen::createCube(10.0f, 0.25f, 60.0f, 40.0f, 8.0f, graphicsDevice);
	if( floor != nullptr ) {
		floor->getXform().setPosition(cc::Vec3f(0.0f, 0.0f, -25.0f));
		models.push_back(floor);
	}

	return success;
}

bool loadTextures() {
	bool success = true;

	ciri::TGA tga0;
	if( !tga0.loadFromFile("data/scifi_metal_wall.tga", true) ) {
		printf("Failed to load tga0.\n");
		success = false;
	} else {
		texture0 = graphicsDevice->createTexture2D(tga0.getWidth(), tga0.getHeight(), ciri::TextureFormat::Color, ciri::TextureFlags::Mipmaps, tga0.getPixels());
		if( nullptr == texture0 ) {
			success = false;
			printf("Failed to set texture0 data from tga0.\n");
		}
		tga0.destroy();
	}

	return success;
}

//fhgbifiokgrijout
// issues to solve:
//    - implement DXTexture2D's writeToTGA
//    - add glGetError check after EVERY gl call (akin to DX having FAILED checks).
//    - go through all todos

bool createSamplers() {
	ciri::SamplerDesc samplerDesc;
	samplerDesc.borderColor[0] = samplerDesc.borderColor[1] = samplerDesc.borderColor[2] = samplerDesc.borderColor[3] = 0.0f;
	samplerDesc.comparisonFunc = ciri::SamplerComparison::Never;
	samplerDesc.filter = ciri::SamplerFilter::Anisotropic;
	samplerDesc.useMipmaps = false;
	samplerDesc.maxAnisotropy = 16;
	samplerDesc.lodBias = 0.0f;
	samplerDesc.maxLod = 3.402823466e+38f;//1000.0f;
	samplerDesc.minLod = 0.0f;//-1000.0f;
	samplerDesc.wrapU = ciri::SamplerWrap::Wrap;
	samplerDesc.wrapV = ciri::SamplerWrap::Wrap;
	samplerDesc.wrapW = ciri::SamplerWrap::Wrap;
	sampler0 = graphicsDevice->createSamplerState(samplerDesc);

	return (sampler0 != nullptr);
}

void cleanup() {
	for( unsigned int i = 0; i < models.size(); ++i ) {
		if( models[i] != nullptr ) {
			delete models[i];
			models[i] = nullptr;
		}
	}

	if( graphicsDevice != nullptr ) {
		graphicsDevice->destroy();
		delete graphicsDevice;
		graphicsDevice = nullptr;
	}

	if( window != nullptr ) {
		window->destroy();
		delete window;
		window = nullptr;
	}
}
