#include "SpritesDemo.hpp"
#include <ciri/core/PNG.hpp>
#include <cc/Common.hpp>
#include <ctime>

SpritesDemo::SpritesDemo()
	: IDemo() {
}

SpritesDemo::~SpritesDemo() {
}

void* SpritesDemo::operator new( size_t i ) {
	return _mm_malloc(i, 16); // allocate aligned to 16
}

void SpritesDemo::operator delete( void* p ) {
	_mm_free(p);
}

DemoConfig SpritesDemo::getConfig() {
	DemoConfig cfg;
	cfg.windowHeight = 720;
	cfg.windowWidth = 1280;
	cfg.windowTitle = "ciri : Sprites Demo";
	return cfg;
}

void SpritesDemo::onInitialize() {
	srand(time(0));
	rand();

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

	_rasterizerState = graphicsDevice()->getDefaultRasterCounterClockwise();
	//ciri::RasterizerDesc rasterizerDesc;
	//rasterizerDesc.cullMode = ciri::CullMode::None;
	//rasterizerDesc.fillMode = ciri::FillMode::Wireframe;
	//_rasterizerState = graphicsDevice()->createRasterizerState(rasterizerDesc);

	// load textures
	ciri::PNG png;
	if( png.loadFromFile("sprites/textures/test.png") && (4 == png.getBytesPerPixel()) ) {
		_texture = graphicsDevice()->createTexture2D(png.getWidth(), png.getHeight(), ciri::TextureFormat::Color, 0, png.getPixels());
	}

	// load balls
	const float MAX_VELOCITY = 1000.0f;
	for( int i = 0; i < 20; ++i ) {
		Ball b;
		b.texture = _texture;
		b.position.x = cc::math::randRange<float>(0.0f, static_cast<float>(window()->getWidth() - _texture->getWidth()));
		b.position.y = cc::math::randRange<float>(0.0f, static_cast<float>(window()->getHeight() - _texture->getHeight()));
		b.velocity.x = cc::math::randRange<float>(-MAX_VELOCITY, MAX_VELOCITY);
		b.velocity.y = cc::math::randRange<float>(-MAX_VELOCITY, MAX_VELOCITY);
		_balls.push_back(b);
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

	if( input()->isKeyDown(ciri::Key::F5) && input()->wasKeyUp(ciri::Key::F5) ) {
		_spritebatch.debugReloadShaders();
	}

	for( auto& ball : _balls ) {
		ball.step(deltaTime);
		ball.collideWalls(0.0f, window()->getWidth(), 0.0f, window()->getHeight());
	}
}

void SpritesDemo::onDraw() {
	const std::shared_ptr<ciri::IGraphicsDevice> device = graphicsDevice();

	device->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);

	static float time = 0.0f;
	time += 0.1f;

	float x = 0.5f;
	float y = 0.5f;

	_spritebatch.begin(_blendState, _samplerState, _depthStencilState, _rasterizerState);
	for( const auto& ball : _balls ) {
		_spritebatch.draw(ball.texture, cc::Vec4f(ball.position.x, ball.position.y, ball.texture->getWidth(), ball.texture->getHeight()), 0.0f, cc::Vec2f(0.0f, 0.0f));
	}
	_spritebatch.end();

	device->present();
}

void SpritesDemo::onUnloadContent() {
	_spritebatch.clean();
}
