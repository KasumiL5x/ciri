#include <ciri/wnd/Window.hpp>
#include <ciri/gfx/GraphicsDeviceFactory.hpp>
#include <ciri/gfx/IShader.hpp>
#include <ciri/gfx/IVertexBuffer.hpp>
#include <ciri/gfx/IIndexBuffer.hpp>
#include <ciri/gfx/IConstantBuffer.hpp>
#include <ciri/gfx/Camera.hpp>
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

	ciri::Camera camera;
	camera.setAspect(1280.0f / 720.0f);
	camera.setPosition(cc::Vec3f(0.0f, 2.0f, 0.0f));
	camera.setTarget(cc::Vec3f(0.0f, 0.0f, -4.0f));

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
	cc::Mat4f world = cc::math::translate(cc::Vec3f(0.0f, 0.0f, -4.0f));
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

	while( window.isOpen() ) {
		ciri::WindowEvent evt;
		while( window.pollEvent(evt) ) {
			if( evt.type == ciri::WindowEvent::Closed ) {
				window.destroy();
			}
		}

		static float time = 0.0f;
		time += 0.0001f;
		static float angle = 0.0f;
		angle += 0.1f;
		if( angle > 360.0f ) {
			angle = (360.0f - angle);
		}
		viewProj = (camera.getProj() * camera.getView());
		world = cc::math::translate(cc::Vec3f(0.0f, 0.0f, -4.0f)) * cc::math::rotate(angle, cc::Vec3f(0.0f, 1.0f, 0.0f));
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
	}

	device->destroy();
	delete device;
	device = nullptr;

	return 0;
}