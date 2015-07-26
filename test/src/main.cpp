#include <ciri/wnd/Window.hpp>
#include <ciri/gfx/GraphicsDeviceFactory.hpp>
#include <ciri/gfx/IShader.hpp>
#include <ciri/gfx/IVertexBuffer.hpp>
#include <ciri/gfx/IIndexBuffer.hpp>
#include <ciri/gfx/IConstantBuffer.hpp>
#include <ciri/gfx/MayaCamera.hpp>
#include <ciri/input/Input.hpp>
#include <ciri/util/Timer.hpp>
#include <ciri/gfx/ITexture2D.hpp>
#include <ciri/util/TGA.hpp>
#include <cc/Vec3.hpp>
#include <cc/Mat4.hpp>
#include <cc/MatrixFunc.hpp>

#if defined(_DEBUG)
	#include <crtdbg.h>
#endif

struct SimpleVertex {
	cc::Vec3f position;
	cc::Vec3f color;

	SimpleVertex() {
	}

	SimpleVertex( const cc::Vec3f& pos, const cc::Vec3f& col )
		: position(pos), color(col) {
	}
};

__declspec(align(16))
struct ShaderData {
	cc::Mat4f xform;
};

void createCube( float size, SimpleVertex* vb, int* ib ) {
	vb[0] = SimpleVertex(cc::Vec3f(-1.0f, -1.0f,  1.0f), cc::Vec3f(1.0f, 0.0f, 0.0f));
	vb[1] = SimpleVertex(cc::Vec3f( 1.0f, -1.0f,  1.0f), cc::Vec3f(0.0f, 1.0f, 0.0f));
	vb[2] = SimpleVertex(cc::Vec3f( 1.0,   1.0,   1.0),  cc::Vec3f(0.0f, 0.0f, 1.0f));
	vb[3] = SimpleVertex(cc::Vec3f(-1.0,   1.0,   1.0),  cc::Vec3f(1.0f, 1.0f, 1.0f));
	vb[4] = SimpleVertex(cc::Vec3f(-1.0,  -1.0,  -1.0),  cc::Vec3f(1.0f, 0.0f, 0.0f));
	vb[5] = SimpleVertex(cc::Vec3f( 1.0,  -1.0,  -1.0),  cc::Vec3f(0.0f, 1.0f, 0.0f));
	vb[6] = SimpleVertex(cc::Vec3f( 1.0,   1.0,  -1.0),  cc::Vec3f(0.0f, 0.0f, 1.0f));
	vb[7] = SimpleVertex(cc::Vec3f(-1.0,   1.0,  -1.0),  cc::Vec3f(1.0f, 1.0f, 1.0f));

	ib[0]=0; ib[1]=1; ib[2]=2;
	ib[3]=2; ib[4]=3; ib[5]=0;

	ib[6]=3; ib[7]=2; ib[8]=6;
	ib[9]=6; ib[10]=7; ib[11]=3;

	ib[12]=7; ib[13]=6; ib[14]=5;
	ib[15]=5; ib[16]=4; ib[17]=7;

	ib[18]=4; ib[19]=5; ib[20]=1;
	ib[21]=1; ib[22]=0; ib[23]=4;

	ib[24]=4; ib[25]=0; ib[26]=3;
	ib[27]=3; ib[28]=7; ib[29]=4;
	
	ib[30]=1; ib[31]=5; ib[32]=6;
	ib[33]=6; ib[34]=2; ib[35]=1;
}

int main() {
	#if defined(_DEBUG)
		int debugFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
		debugFlag|= _CRTDBG_LEAK_CHECK_DF;
		debugFlag |= _CRTDBG_CHECK_ALWAYS_DF;
		_CrtSetDbgFlag(debugFlag);
	#endif

	ciri::MayaCamera camera;
	camera.setAspect(1280.0f / 720.0f);
	camera.setPlanes(0.1f, 1000.0f);
	camera.setYaw(0.0f);
	camera.setPitch(9.0f);
	camera.setOffset(4.0f);
	camera.setSensitivity(100.0f, 50.0f, 10.0f);
	camera.setLerpStrength(100.0f);
	camera.setTarget(cc::Vec3f(0.0f, 0.0f, -0.0f));
	camera.resetPosition();

	ciri::Window window;
	window.create(1280, 720);

	ciri::IGraphicsDevice* device = ciri::GraphicsDeviceFactory::create(ciri::GraphicsDeviceFactory::OpenGL);
	if( !device->create(&window) ) {
		printf("Failed to initialize graphics device.\n");
		return -1;
	} else {
		printf("Graphics device initialized.\n");
	}

	ciri::IShader* shader = device->createShader();
	shader->addVertexShader("data/simple_vs.glsl");
	shader->addPixelShader("data/simple_ps.glsl");
	shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Position, 0));
	shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Color, 0));
	if( ciri::err::failed(shader->build()) ) {
		printf("Failed to build shader: %s.\n", shader->getLastError());
	} else {
		printf("Shader built.\n");
	}

	SimpleVertex vertices[8];
	int indices[36];
	createCube(1.0f, vertices, indices);

	ciri::IVertexBuffer* vertexBuffer = device->createVertexBuffer();
	if( !vertexBuffer->set(vertices, sizeof(SimpleVertex), 8, false) ) {
		printf("Failed to create vertex buffer.\n");
	} else {
		printf("Created vertex buffer.\n");
	}

	ciri::IIndexBuffer* indexBuffer = device->createIndexBuffer();
	if( !indexBuffer->set(indices, 36, false) ) {
		printf("Failed to create index buffer.\n");
	} else {
		printf("Created index buffer.\n");
	}

	ShaderData shaderData;
	cc::Mat4f viewProj = (camera.getProj() * camera.getView());
	cc::Mat4f world = cc::math::translate(cc::Vec3f(0.0f, 0.0f, -0.0f));
	cc::Mat4f xform = viewProj * world;
	shaderData.xform = xform;
	ciri::IConstantBuffer* constantBuffer = device->createConstantBuffer();
	if( ciri::err::failed(constantBuffer->setData(sizeof(shaderData), &shaderData)) ) {
		printf("Failed to create constant buffer.\n");
	} else {
		printf("Created constant buffer.\n");
	}
	if( ciri::err::failed(shader->addConstants(constantBuffer, "TestConstants", ciri::ShaderType::Vertex)) ) {
		printf("Failed to assign constant buffer to shader.\n");
	} else {
		printf("Assigned constant buffer to shader.\n");
	}

	// create a texture
	ciri::ITexture2D* texture2D = device->createTexture2D();
	ciri::TGA tgaFile;
	if( !tgaFile.loadFromFile("data/nyan.tga") ) {
		printf("Failed to load texture from TGA file.\n");
	} else {
		printf("Loaded texture from TGA file.\n");
		if( !texture2D->setData(0, 0, tgaFile.getWidth(), tgaFile.getHeight(), tgaFile.getPixels(), ciri::TextureFormat::Color) ) {
			printf("Failed to set texture2d data.\n");
		} else {
			printf("Set texture2d data.\n");
		}
	}

	// mouse and keyboard states
	ciri::KeyboardState currKeyState; ciri::Input::getKeyboardState(&currKeyState);
	ciri::KeyboardState prevKeyState = currKeyState;
	ciri::MouseState currMouseState; ciri::Input::getMouseState(&currMouseState, &window);
	ciri::MouseState prevMouseState = currMouseState;

	// delta timer
	ciri::Timer timer;
	timer.start();
	double lastTime = 0.0;

	while( window.isOpen() ) {
		// event processing
		ciri::WindowEvent evt;
		while( window.pollEvent(evt) ) {
			if( evt.type == ciri::WindowEvent::Closed ) {
				window.destroy();
			}
		}

		// delta time
		const double currTime = timer.getElapsedMicroseconds();
		const double deltaTime = (currTime - lastTime) * 0.000001;
		lastTime = currTime;

		// update current input states
		ciri::Input::getKeyboardState(&currKeyState);
		ciri::Input::getMouseState(&currMouseState, &window);

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
				const float dy = static_cast<float>(currMouseState.y) - static_cast<float>(prevMouseState.y);
				camera.dolly(dy * deltaTime);
			}

			// pan
			if( currMouseState.isButtonDown(ciri::MouseButton::Middle) ) {
				const float dx = static_cast<float>(currMouseState.x) - static_cast<float>(prevMouseState.x);
				const float dy = static_cast<float>(currMouseState.y) - static_cast<float>(prevMouseState.y);
				camera.pan(dx * deltaTime, -dy * deltaTime);
			}
		}
		camera.update(deltaTime);

		//static float time = 0.0f;
		//time += 0.0001f;
		//static float angle = 0.0f;
		//angle += 0.1f;
		//if( angle > 360.0f ) {
			//angle = (360.0f - angle);
		//}
		viewProj = (camera.getProj() * camera.getView());
		world = cc::math::translate(cc::Vec3f(0.0f, 0.0f, -0.0f));// * cc::math::rotate(angle, cc::Vec3f(0.0f, 1.0f, 0.0f));
		cc::Mat4f xform = viewProj * world;
		shaderData.xform = xform;
		if( ciri::err::failed(constantBuffer->setData(sizeof(shaderData), &shaderData)) ) {
			printf("Failed to update constant buffer.\n");
		}

		device->clear();
		device->applyShader(shader);
		device->setVertexBuffer(vertexBuffer);
		device->setIndexBuffer(indexBuffer);
		device->drawIndexed(ciri::PrimitiveTopology::TriangleList, indexBuffer->getIndexCount());
		device->present();

		// update previous inputs to current inputs
		prevKeyState = currKeyState;
		prevMouseState = currMouseState;
	}

	tgaFile.destroy();

	device->destroy();
	delete device;
	device = nullptr;

	return 0;
}