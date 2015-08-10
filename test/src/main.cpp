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
const ciri::GraphicsDeviceFactory::DeviceType GRAPHICS_DEVICE_TYPE = ciri::GraphicsDeviceFactory::DirectX;
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

		graphicsDevice->clear(ciri::ClearFlags::ColorDepth);

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
			// render all models with the simple shader
			graphicsDevice->applyShader(simpleShader);
			graphicsDevice->setTexture2D(0, texture0, ciri::ShaderStage::Pixel);
			graphicsDevice->setSamplerState(0, sampler0, ciri::ShaderStage::Pixel);
			for( unsigned int i = 0; i < models.size(); ++i ) {
				Model* mdl = models[i];

				// update constant buffer for this object
				simpleConstants.world = mdl->getXform().getWorld();
				simpleConstants.xform = simpleConstants.world * cameraViewProj;
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
				simpleConstants.xform = simpleConstants.world * cameraViewProj;
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
	//if( !loadModelFromObj("data/controller.obj", testModel) ) {
	//	delete testModel;
	//	testModel = nullptr;
	//	success = false;
	//	printf("Failed to load test model.\n");
	//} else {
	//	models.push_back(testModel);
	//}

	// create a floor
	Model* floor = modelgen::createCube(10.0f, 0.25f, 10.0f, 4.0f, 4.0f, graphicsDevice);
	if( floor != nullptr ) {
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
		texture0 = graphicsDevice->createTexture2D();
		if( ciri::err::failed(texture0->setData(0, 0, tga0.getWidth(), tga0.getHeight(), tga0.getPixels(), ciri::TextureFormat::Color)) ) {
			texture0->destroy();
			delete texture0;
			texture0 = nullptr;
			success = false;
			printf("Failed to set texture0 data from tga0.\n");
		}
		tga0.destroy();
	}

	return success;
}

//fhgbifiokgrijout
// issues to solve:
//    - mipmaps need to be (optionaly, through a flag) generated for both DX and GL
//    - bilinear and trilinear sampling in DX seem to have no effect, whereas GL works fine (look this up online)
//    - a million other things
// here's some links i had open earlier:
// http://gamedev.stackexchange.com/questions/66231/set-sampler-states-linear-bilinear-trilinear-filtering-interpolation
// https://msdn.microsoft.com/en-us/library/windows/desktop/ff476426(v=vs.85).aspx
// https://msdn.microsoft.com/en-us/library/windows/desktop/ff476499(v=vs.85).aspx
// http://stackoverflow.com/questions/5321330/how-do-you-use-checkmultisamplequalitylevels-and-enable-multisampling
// https://msdn.microsoft.com/en-us/library/windows/desktop/bb509695(v=vs.85).aspx
// http://www.directxtutorial.com/Lesson.aspx?lessonid=11-4-2

bool createSamplers() {
	ciri::SamplerDesc samplerDesc;
	samplerDesc.borderColor[0] = samplerDesc.borderColor[1] = samplerDesc.borderColor[2] = samplerDesc.borderColor[3] = 0.0f;
	samplerDesc.comparisonFunc = ciri::SamplerComparison::Never;
	samplerDesc.filter = ciri::SamplerFilter::Bilinear;
	samplerDesc.maxAnisotropy = 16;
	samplerDesc.lodBias = 0.0f;
	samplerDesc.maxLod = 1000.0f;
	samplerDesc.minLod = -1000.0f;
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

//#include <ciri/wnd/Window.hpp>
//#include <ciri/gfx/GraphicsDeviceFactory.hpp>
//#include <ciri/gfx/IShader.hpp>
//#include <ciri/gfx/IVertexBuffer.hpp>
//#include <ciri/gfx/IIndexBuffer.hpp>
//#include <ciri/gfx/IConstantBuffer.hpp>
//#include <ciri/gfx/MayaCamera.hpp>
//#include <ciri/input/Input.hpp>
//#include <ciri/util/Timer.hpp>
//#include <ciri/gfx/ITexture2D.hpp>
//#include <ciri/gfx/IRenderTarget2D.hpp>
//#include <ciri/util/TGA.hpp>
//#include <cc/Vec3.hpp>
//#include <cc/Mat4.hpp>
//#include <cc/MatrixFunc.hpp>
//#if defined(_DEBUG)
//	#include <crtdbg.h>
//#endif
//
//struct SimpleVertex {
//	cc::Vec3f position;
//	cc::Vec2f texcoord;
//
//	SimpleVertex() {
//	}
//
//	SimpleVertex( const cc::Vec3f& pos, const cc::Vec2f& tex )
//		: position(pos), texcoord(tex) {
//	}
//};
//
//__declspec(align(16))
//struct ShaderData {
//	cc::Mat4f xform;
//};
//
//void createCube( float size, float uvScale, SimpleVertex* vb, int* ib ) {
//	vb[0]  = SimpleVertex(cc::Vec3f(-0.5f*size, -0.5f*size,  0.5f*size), cc::Vec2f(0.0f * uvScale,  0.0f * uvScale));
//	vb[1]  = SimpleVertex(cc::Vec3f( 0.5f*size, -0.5f*size,  0.5f*size), cc::Vec2f(1.0f * uvScale,  0.0f * uvScale));
//	vb[2]  = SimpleVertex(cc::Vec3f( 0.5f*size,  0.5f*size,  0.5f*size), cc::Vec2f(1.0f * uvScale,  1.0f * uvScale));
//	vb[3]  = SimpleVertex(cc::Vec3f(-0.5f*size,  0.5f*size,  0.5f*size), cc::Vec2f(0.0f * uvScale,  1.0f * uvScale));
//	vb[4]  = SimpleVertex(cc::Vec3f(-0.5f*size, -0.5f*size, -0.5f*size), cc::Vec2f(0.0f * uvScale,  0.0f * uvScale));
//	vb[5]  = SimpleVertex(cc::Vec3f(-0.5f*size,  0.5f*size, -0.5f*size), cc::Vec2f(1.0f * uvScale,  0.0f * uvScale));
//	vb[6]  = SimpleVertex(cc::Vec3f( 0.5f*size,  0.5f*size, -0.5f*size), cc::Vec2f(1.0f * uvScale,  1.0f * uvScale));
//	vb[7]  = SimpleVertex(cc::Vec3f( 0.5f*size, -0.5f*size, -0.5f*size), cc::Vec2f(0.0f * uvScale,  1.0f * uvScale));
//	vb[8]  = SimpleVertex(cc::Vec3f(-0.5f*size,  0.5f*size, -0.5f*size), cc::Vec2f(0.0f * uvScale,  0.0f * uvScale));
//	vb[9]  = SimpleVertex(cc::Vec3f(-0.5f*size,  0.5f*size,  0.5f*size), cc::Vec2f(1.0f * uvScale,  0.0f * uvScale));
//	vb[10] = SimpleVertex(cc::Vec3f( 0.5f*size,  0.5f*size,  0.5f*size), cc::Vec2f(1.0f * uvScale,  1.0f * uvScale));
//	vb[11] = SimpleVertex(cc::Vec3f( 0.5f*size,  0.5f*size, -0.5f*size), cc::Vec2f(0.0f * uvScale,  1.0f * uvScale));
//	vb[12] = SimpleVertex(cc::Vec3f(-0.5f*size, -0.5f*size, -0.5f*size), cc::Vec2f(0.0f * uvScale,  0.0f * uvScale));
//	vb[13] = SimpleVertex(cc::Vec3f( 0.5f*size, -0.5f*size, -0.5f*size), cc::Vec2f(1.0f * uvScale,  0.0f * uvScale));
//	vb[14] = SimpleVertex(cc::Vec3f( 0.5f*size, -0.5f*size,  0.5f*size), cc::Vec2f(1.0f * uvScale,  1.0f * uvScale));
//	vb[15] = SimpleVertex(cc::Vec3f(-0.5f*size, -0.5f*size,  0.5f*size), cc::Vec2f(0.0f * uvScale,  1.0f * uvScale));
//	vb[16] = SimpleVertex(cc::Vec3f( 0.5f*size, -0.5f*size, -0.5f*size), cc::Vec2f(0.0f * uvScale,  0.0f * uvScale));
//	vb[17] = SimpleVertex(cc::Vec3f( 0.5f*size,  0.5f*size, -0.5f*size), cc::Vec2f(1.0f * uvScale,  0.0f * uvScale));
//	vb[18] = SimpleVertex(cc::Vec3f( 0.5f*size,  0.5f*size,  0.5f*size), cc::Vec2f(1.0f * uvScale,  1.0f * uvScale));
//	vb[19] = SimpleVertex(cc::Vec3f( 0.5f*size, -0.5f*size,  0.5f*size), cc::Vec2f(0.0f * uvScale,  1.0f * uvScale));
//	vb[20] = SimpleVertex(cc::Vec3f(-0.5f*size, -0.5f*size, -0.5f*size), cc::Vec2f(0.0f * uvScale,  0.0f * uvScale));
//	vb[21] = SimpleVertex(cc::Vec3f(-0.5f*size, -0.5f*size,  0.5f*size), cc::Vec2f(1.0f * uvScale,  0.0f * uvScale));
//	vb[22] = SimpleVertex(cc::Vec3f(-0.5f*size,  0.5f*size,  0.5f*size), cc::Vec2f(1.0f * uvScale,  1.0f * uvScale));
//	vb[23] = SimpleVertex(cc::Vec3f(-0.5f*size,  0.5f*size, -0.5f*size), cc::Vec2f(0.0f * uvScale,  1.0f * uvScale));
//
//	ib[0]=0;   ib[1]=1;   ib[2]=2;   ib[3]=0;   ib[4]=2;   ib[5]=3;
//	ib[6]=4;   ib[7]=5;   ib[8]=6;   ib[9]=4;   ib[10]=6;  ib[11]=7;
//	ib[12]=8;  ib[13]=9;  ib[14]=10; ib[15]=8;  ib[16]=10; ib[17]=11;
//	ib[18]=12; ib[19]=13; ib[20]=14; ib[21]=12; ib[22]=14; ib[23]=15;
//	ib[24]=16; ib[25]=17; ib[26]=18; ib[27]=16; ib[28]=18; ib[29]=19;
//	ib[30]=20; ib[31]=21; ib[32]=22; ib[33]=20; ib[34]=22; ib[35]=23;
//}
//
//int main() {
//	#if defined(_DEBUG)
//		int debugFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
//		debugFlag|= _CRTDBG_LEAK_CHECK_DF;
//		debugFlag |= _CRTDBG_CHECK_ALWAYS_DF;
//		_CrtSetDbgFlag(debugFlag);
//	#endif
//
//	ciri::MayaCamera camera;
//	camera.setAspect(1280.0f / 720.0f);
//	camera.setPlanes(0.1f, 1000.0f);
//	camera.setYaw(0.0f);
//	camera.setPitch(9.0f);
//	camera.setOffset(4.0f);
//	camera.setSensitivity(100.0f, 50.0f, 10.0f);
//	camera.setLerpStrength(100.0f);
//	camera.setTarget(cc::Vec3f(0.0f, 0.0f, -0.0f));
//	camera.resetPosition();
//
//	ciri::Window window;
//	window.create(1280, 720);
//
//	ciri::IGraphicsDevice* device = ciri::GraphicsDeviceFactory::create(ciri::GraphicsDeviceFactory::OpenGL);
//	if( !device->create(&window) ) {
//		printf("Failed to initialize graphics device.\n");
//		return -1;
//	} else {
//		printf("Graphics device initialized.\n");
//	}
//
//	ciri::IShader* shader = device->createShader();
//	shader->addVertexShader("data/simple_vs.glsl");
//	shader->addPixelShader("data/simple_ps.glsl");
//	shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Position, 0));
//	shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float2, ciri::VertexUsage::Texcoord, 0));
//	if( ciri::err::failed(shader->build()) ) {
//		printf("Failed to build shader: %s.\n", shader->getLastError());
//	} else {
//		printf("Shader built.\n");
//	}
//
//	const unsigned int VERTEX_COUNT = 24;
//	const unsigned int INDEX_COUNT = 36;
//	SimpleVertex vertices[VERTEX_COUNT];
//	int indices[INDEX_COUNT];
//	createCube(1.0f, 1.0f, vertices, indices);
//
//	ciri::IVertexBuffer* vertexBuffer = device->createVertexBuffer();
//	if( !vertexBuffer->set(vertices, sizeof(SimpleVertex), VERTEX_COUNT, false) ) {
//		printf("Failed to create vertex buffer.\n");
//	} else {
//		printf("Created vertex buffer.\n");
//	}
//
//	ciri::IIndexBuffer* indexBuffer = device->createIndexBuffer();
//	if( !indexBuffer->set(indices, INDEX_COUNT, false) ) {
//		printf("Failed to create index buffer.\n");
//	} else {
//		printf("Created index buffer.\n");
//	}
//
//	ShaderData shaderData;
//	cc::Mat4f viewProj = (camera.getProj() * camera.getView());
//	cc::Mat4f world = cc::math::translate(cc::Vec3f(0.0f, 0.0f, -0.0f));
//	cc::Mat4f xform = viewProj * world;
//	shaderData.xform = xform;
//	ciri::IConstantBuffer* constantBuffer = device->createConstantBuffer();
//	if( ciri::err::failed(constantBuffer->setData(sizeof(shaderData), &shaderData)) ) {
//		printf("Failed to create constant buffer.\n");
//	} else {
//		printf("Created constant buffer.\n");
//	}
//	if( ciri::err::failed(shader->addConstants(constantBuffer, "TestConstants", ciri::ShaderType::Vertex)) ) {
//		printf("Failed to assign constant buffer to shader.\n");
//	} else {
//		printf("Assigned constant buffer to shader.\n");
//	}
//
//	// create a texture
//	ciri::ITexture2D* texture2D = device->createTexture2D();
//	ciri::TGA tgaFile;
//	if( !tgaFile.loadFromFile("data/dirt.tga", true) || tgaFile.getFormat() != ciri::TGA::RGBA ) {
//		printf("Failed to load texture from TGA file.\n");
//	} else {
//		printf("Loaded texture from TGA file.\n");
//		if( !texture2D->setData(0, 0, tgaFile.getWidth(), tgaFile.getHeight(), tgaFile.getPixels(), ciri::TextureFormat::Color) ) {
//			printf("Failed to set texture2d data.\n");
//		} else {
//			printf("Set texture2d data.\n");
//			device->setTexture2D(0, texture2D, ciri::ShaderStage::Pixel);
//		}
//	}
//
//	// create a sampler state
//	ciri::SamplerDesc samplerDesc;
//	samplerDesc.borderColor[0] = samplerDesc.borderColor[1] = samplerDesc.borderColor[2] = samplerDesc.borderColor[3] = 0.0f;
//	samplerDesc.comparisonFunc = ciri::SamplerComparison::Never;
//	samplerDesc.filter = ciri::SamplerFilter::Bilinear;
//	samplerDesc.maxAnisotropy = 16.0f;
//	samplerDesc.lodBias = 0.0f;
//	samplerDesc.maxLod = 1000.0f;
//	samplerDesc.minLod = -1000.0f;
//	samplerDesc.wrapU = ciri::SamplerWrap::Wrap;
//	samplerDesc.wrapV = ciri::SamplerWrap::Wrap;
//	samplerDesc.wrapW = ciri::SamplerWrap::Wrap;
//	ciri::ISamplerState* samplerState = device->createSamplerState(samplerDesc);
//	device->setSamplerState(0, samplerState, ciri::ShaderStage::Pixel);
//
//	// render target
//	ciri::IRenderTarget2D* renderTarget = device->createRenderTarget2D(64, 64, ciri::TextureFormat::RGB32_Float);
//
//	// mouse and keyboard states
//	ciri::KeyboardState currKeyState; ciri::Input::getKeyboardState(&currKeyState);
//	ciri::KeyboardState prevKeyState = currKeyState;
//	ciri::MouseState currMouseState; ciri::Input::getMouseState(&currMouseState, &window);
//	ciri::MouseState prevMouseState = currMouseState;
//
//	// delta timer
//	ciri::Timer timer;
//	timer.start();
//	double lastTime = 0.0;
//
//	while( window.isOpen() ) {
//		// event processing
//		ciri::WindowEvent evt;
//		while( window.pollEvent(evt) ) {
//			if( evt.type == ciri::WindowEvent::Closed ) {
//				window.destroy();
//			}
//		}
//
//		// delta time
//		const double currTime = timer.getElapsedMicroseconds();
//		const float deltaTime = static_cast<float>((currTime - lastTime) * 0.000001);
//		lastTime = currTime;
//
//		// update current input states
//		ciri::Input::getKeyboardState(&currKeyState);
//		ciri::Input::getMouseState(&currMouseState, &window);
//
//		// close dat shit
//		if( currKeyState.isKeyDown(ciri::Keyboard::Escape) ) {
//			window.destroy();
//			break;
//		}
//
//		// camera movement
//		if( currKeyState.isKeyDown(ciri::Keyboard::LAlt) ) {
//			// rotation
//			if( currMouseState.isButtonDown(ciri::MouseButton::Left) ) {
//				const float dx = static_cast<float>(currMouseState.x) - static_cast<float>(prevMouseState.x);
//				const float dy = static_cast<float>(currMouseState.y) - static_cast<float>(prevMouseState.y);
//				camera.rotateYaw(-dx * deltaTime);
//				camera.rotatePitch(-dy * deltaTime);
//			}
//
//			// dolly
//			if( currMouseState.isButtonDown(ciri::MouseButton::Right) ) {
//				const float dy = static_cast<float>(currMouseState.y) - static_cast<float>(prevMouseState.y);
//				camera.dolly(dy * deltaTime);
//			}
//
//			// pan
//			if( currMouseState.isButtonDown(ciri::MouseButton::Middle) ) {
//				const float dx = static_cast<float>(currMouseState.x) - static_cast<float>(prevMouseState.x);
//				const float dy = static_cast<float>(currMouseState.y) - static_cast<float>(prevMouseState.y);
//				camera.pan(dx * deltaTime, -dy * deltaTime);
//			}
//		}
//		camera.update(deltaTime);
//
//		world = cc::math::translate(cc::Vec3f(0.0f, 0.0f, -0.0f));// * cc::math::rotate(angle, cc::Vec3f(0.0f, 1.0f, 0.0f));
//
//		///// BEGIN OLD CODE /////
//		//viewProj = (camera.getProj() * camera.getView());
//		//cc::Mat4f xform = viewProj * world;
//		//shaderData.xform = xform;
//		//if( ciri::err::failed(constantBuffer->setData(sizeof(shaderData), &shaderData)) ) {
//		//	printf("Failed to update constant buffer.\n");
//		//}
//		//device->clear();
//		//device->setTexture2D(0, texture2D, ciri::ShaderStage::Pixel);
//		//device->applyShader(shader);
//		//device->setVertexBuffer(vertexBuffer);
//		//device->setIndexBuffer(indexBuffer);
//		//device->drawIndexed(ciri::PrimitiveTopology::TriangleList, indexBuffer->getIndexCount());
//		//device->present();
//		///// END OLD CODE /////
//
//		// update camera matrix
//		viewProj = (camera.getProj() * camera.getView());
//		cc::Mat4f xform = viewProj * world;
//		shaderData.xform = xform;
//		if( ciri::err::failed(constantBuffer->setData(sizeof(shaderData), &shaderData)) ) {
//			printf("Failed to update constant buffer.\n");
//		}
//		// render the scene to texture
//		ciri::IRenderTarget2D* renderTargets = { renderTarget };
//		device->setRenderTargets(&renderTargets, 1);
//		float firstClearColor[4] = {0.3f, 0.3f, 0.3f, 1.0f};
//		device->clear(ciri::ClearFlags::Color, &firstClearColor[0]);
//		device->setTexture2D(0, texture2D, ciri::ShaderStage::Pixel);
//		device->applyShader(shader);
//		device->setVertexBuffer(vertexBuffer);
//		device->setIndexBuffer(indexBuffer);
//		device->drawIndexed(ciri::PrimitiveTopology::TriangleList, indexBuffer->getIndexCount());
//		//// render to the screen using the rendertarget as a texture
//		device->restoreDefaultRenderTargets();
//		float secondClearColor[4] = {0.39f, 0.58f, 0.93f, 1.0f};
//		device->clear(ciri::ClearFlags::ColorDepth, secondClearColor);
//		device->setTexture2D(0, renderTarget->getTexture2D(), ciri::ShaderStage::Pixel);
//		device->applyShader(shader);
//		device->setVertexBuffer(vertexBuffer);
//		device->setIndexBuffer(indexBuffer);
//		device->drawIndexed(ciri::PrimitiveTopology::TriangleList, indexBuffer->getIndexCount());
//		device->present();
//
//		// update previous inputs to current inputs
//		prevKeyState = currKeyState;
//		prevMouseState = currMouseState;
//	}
//
//	tgaFile.destroy();
//
//	device->destroy();
//	delete device;
//	device = nullptr;
//
//	return 0;
//}