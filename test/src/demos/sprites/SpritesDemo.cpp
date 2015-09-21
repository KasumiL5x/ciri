#include "SpritesDemo.hpp"
#include <ciri/core/PNG.hpp>

SpritesDemo::SpritesDemo()
	: IDemo() {
}

SpritesDemo::~SpritesDemo() {
}

DemoConfig SpritesDemo::getConfig() {
	DemoConfig cfg;
	cfg.windowHeight = 720;
	cfg.windowWidth = 1280;
	cfg.windowTitle = "ciri : Sprites Demo";
	return cfg;
}

void SpritesDemo::onInitialize() {
	// print driver information
	printf("Device: %s\n", graphicsDevice()->getGpuName());
	printf("API: %s\n", graphicsDevice()->getApiInfo());

	// create spritebatch
	if( !_spritebatch.create(graphicsDevice()) ) {
		printf("Failed to initialize SpriteBatch.\n");
	} else {
		printf("Initialized SpriteBatch.\n");
	}

	// create states
	//ciri::BlendDesc blendDesc;
	_blendState = graphicsDevice()->getDefaultBlendAlpha(); //graphicsDevice()->createBlendState(blendDesc);

	ciri::SamplerDesc samplerDesc;
	_samplerState = graphicsDevice()->createSamplerState(samplerDesc);

	//ciri::DepthStencilDesc depthStencilDesc;
	_depthStencilState = graphicsDevice()->getDefaultDepthStencilNone(); //graphicsDevice()->createDepthStencilState(depthStencilDesc);

	//ciri::RasterizerDesc rasterizerDesc;
	_rasterizerState = graphicsDevice()->getDefaultRasterCounterClockwise(); //graphicsDevice()->createRasterizerState(rasterizerDesc);

	// load textures
	ciri::PNG png;
	if( png.loadFromFile("sprites/textures/test.png") && (4 == png.getBytesPerPixel()) ) {
		_texture = graphicsDevice()->createTexture2D(png.getWidth(), png.getHeight(), ciri::TextureFormat::Color, 0, png.getPixels());
	}
}

void SpritesDemo::onLoadContent() {
}

void SpritesDemo::onEvent( ciri::WindowEvent evt ) {
	switch( evt.type) {
		case ciri::WindowEvent::Resized: {
			if( graphicsDevice()->resize() != ciri::ErrorCode::CIRI_OK ) {
				printf("Failed to resize default render targets.\n");
			}
			break;
		}
	}
}

void SpritesDemo::onUpdate( double deltaTime, double elapsedTime ) {
	// check for close w/ escape
	if( input()->isKeyDown(ciri::Key::Escape) ) {
		this->gtfo();
		return;
	}
}

void SpritesDemo::onDraw() {
	const std::shared_ptr<ciri::IGraphicsDevice> device = graphicsDevice();

	device->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);

	static float time = 0.0f;
	time += 0.1f;

	float x = (1280.0f * 0.5f) + (sin(time * 50.0f) * 15.0f);
	float y = (720.0f  * 0.5f) + (cos(time * 50.0f) * 15.0f);

	_spritebatch.begin(_blendState, _samplerState, _depthStencilState, _rasterizerState);
	_spritebatch.draw(_texture, cc::Vec2f(x, y), cc::Vec2f(0.25f, 0.25f));
	const float width = static_cast<float>(window()->getWidth());
	const float height = static_cast<float>(window()->getHeight());
	_spritebatch.end(width, height);

	device->present();
}

void SpritesDemo::onUnloadContent() {
}
