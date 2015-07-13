#include <ciri/wnd/Window.hpp>
#include <ciri/gfx/GraphicsDeviceFactory.hpp>
#include <ciri/gfx/IShader.hpp>

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
	shader->addInputElement(ciri::VertexElement(0, ciri::VertexFormat::Float3, ciri::VertexUsage::Position, 0));
	if( !shader->build() ) {
		printf("Failed to build shader: %s.\n", shader->getLastError());
	} else {
		printf("Shader built.\n");
	}

	ciri::IVertexBuffer* vertexBuffer = device->createVertexBuffer();
	// ...

	while( window.isOpen() ) {
		ciri::WindowEvent evt;
		while( window.pollEvent(evt) ) {
			if( evt.type == ciri::WindowEvent::Closed ) {
				window.destroy();
			}

			if( evt.type == ciri::WindowEvent::Resized ) {
				//printf("w: %d; h: %d\n", evt.size.width, evt.size.height);
			}

			if( evt.type == ciri::WindowEvent::FocusGained ) {
				//printf("focus gained\n");
			}

			if( evt.type == ciri::WindowEvent::FocusLost ) {
				//printf("focus lost\n");
			}
		}

		// set shader
		device->applyShader(shader);

		// render and flip
		device->present();
	}

	device->destroy();
	delete device;
	device = nullptr;

	return 0;
}