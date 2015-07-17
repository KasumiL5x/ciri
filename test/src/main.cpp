#include <ciri/wnd/Window.hpp>
#include <ciri/gfx/GraphicsDeviceFactory.hpp>
#include <ciri/gfx/IShader.hpp>
#include <ciri/gfx/IVertexBuffer.hpp>
#include <cc/Vec3.hpp>

struct SimpleVertex {
	cc::Vec3f position;
	cc::Vec3f color;
};

int main() {
	ciri::Window window;
	window.create(1280, 720);

	ciri::IGraphicsDevice* device = ciri::GraphicsDeviceFactory::create(ciri::GraphicsDeviceFactory::DirectX);
	if( !device->create(&window) ) {
		printf("Failed to initialize graphics device.\n");
		return -1;
	} else {
		printf("Graphics device initialized.\n");
	}

	ciri::IShader* shader = device->createShader();
	shader->addVertexShader("data/simple_vs.hlsl");
	shader->addPixelShader("data/simple_ps.hlsl");
	shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Position, 0));
	shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Color, 0));
	if( ciri::err::failed(shader->build()) ) {
		printf("Failed to build shader: %s.\n", shader->getLastError());
	} else {
		printf("Shader built.\n");
	}

	ciri::IVertexBuffer* vertexBuffer = device->createVertexBuffer();
	SimpleVertex vertices[] = {
		{cc::Vec3f( 0.0f,  0.5f, 0.5f), cc::Vec3f(1.0f, 0.0f, 0.0f) },
		{cc::Vec3f( 0.5f, -0.5f, 0.5f), cc::Vec3f(0.0f, 1.0f, 0.0f) },
		{cc::Vec3f(-0.5f, -0.5f, 0.5f), cc::Vec3f(0.0f, 0.0f, 1.0f) },
	};
	if( !vertexBuffer->set(vertices, sizeof(SimpleVertex), 3, false) ) {
		printf("Failed to create vertex buffer.\n");
	} else {
		printf("Created vertex buffer.");
	}

	while( window.isOpen() ) {
		ciri::WindowEvent evt;
		while( window.pollEvent(evt) ) {
			if( evt.type == ciri::WindowEvent::Closed ) {
				window.destroy();
			}
		}

		device->clear();

		device->applyShader(shader);
		device->setVertexBuffer(vertexBuffer);
		device->drawArrays(ciri::PrimitiveTopology::TriangleList, 3, 0);

		device->present();
	}

	device->destroy();
	delete device;
	device = nullptr;

	return 0;
}