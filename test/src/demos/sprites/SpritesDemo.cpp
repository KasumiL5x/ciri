#include "SpritesDemo.hpp"
#include <ciri/core/PNG.hpp>
#include <cc/Common.hpp>
#include <ctime>

SpritesDemo::SpritesDemo()
	: IDemo(), _ballsMoving(false) {
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
	 //graphicsDevice()->createBlendState(blendDesc);
	_blendState = graphicsDevice()->getDefaultBlendAlpha();

	ciri::SamplerDesc samplerDesc;
	_samplerState = graphicsDevice()->createSamplerState(samplerDesc);

	//ciri::DepthStencilDesc depthStencilDesc;
	//graphicsDevice()->createDepthStencilState(depthStencilDesc);
	_depthStencilState = graphicsDevice()->getDefaultDepthStencilNone();//Default();

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
	const float MAX_VELOCITY = 500.0f;
	for( int i = 0; i < 20; ++i ) {
		Ball b;
		b.texture = _texture;
		b.position.x = cc::math::randRange<float>(0.0f, static_cast<float>(window()->getWidth() - _texture->getWidth()));
		b.position.y = cc::math::randRange<float>(0.0f, static_cast<float>(window()->getHeight() - _texture->getHeight()));
		b.velocity.x = cc::math::randRange<float>(-MAX_VELOCITY, MAX_VELOCITY);
		b.velocity.y = cc::math::randRange<float>(-MAX_VELOCITY, MAX_VELOCITY);
		b.origin = cc::Vec2f(_texture->getWidth() * 0.5f, _texture->getHeight() * 0.5f);
		b.rotation = 0.0f;
		_balls.push_back(b);
	}
}

void SpritesDemo::onLoadContent() {
	ciri::PNG carPng;
	if( carPng.loadFromFile("sprites/textures/car.png") && (4 == carPng.getBytesPerPixel()) ) {
		_car.setTexture(graphicsDevice()->createTexture2D(carPng.getWidth(), carPng.getHeight(), ciri::TextureFormat::Color, 0, carPng.getPixels()));
		_car.setOrigin(carPng.getWidth() / 2, carPng.getHeight() / 2);
		_car.setPosition(window()->getWidth()/2, window()->getHeight()/2);
	}
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

	// toggle balls update
	if( input()->isKeyDown(ciri::Key::P) && input()->wasKeyUp(ciri::Key::P) ) {
		_ballsMoving = !_ballsMoving;
	}

	if( _ballsMoving ) {
		for( auto& ball : _balls ) {
			ball.velocity.y -= 20.8f;
			ball.step(deltaTime);
			ball.collideWalls(0.0f, window()->getWidth(), 0.0f, window()->getHeight());
			ball.rotation += cc::math::degreesToRadians(ball.velocity.x * deltaTime);
			ball.rotation = cc::math::wrapAngle(ball.rotation, 0.0f, cc::math::degreesToRadians(360.0f));
		}
	}

	// car update
	const float throttleInput = input()->isKeyDown(ciri::Key::Up) ? 10.0f : (input()->isKeyDown(ciri::Key::Down) ? -10.0f : 0.0f);
	const float steerInput = input()->isKeyDown(ciri::Key::Left) ? 1.0f : (input()->isKeyDown(ciri::Key::Right) ? -1.0f : 0.0f);
	_car.update(throttleInput, steerInput, deltaTime);
}

void SpritesDemo::onDraw() {
	const std::shared_ptr<ciri::IGraphicsDevice> device = graphicsDevice();

	device->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);
	
	_spritebatch.begin(_blendState, _samplerState, _depthStencilState, _rasterizerState, SpriteSortMode::Deferred);
	int counter = 0;
	for( const auto& ball : _balls ) {
		counter += 1;
		const float depth = (float)counter / (float)_balls.size();
		_spritebatch.draw(ball.texture, cc::Vec4f(ball.position.x, ball.position.y, ball.texture->getWidth(), ball.texture->getHeight()), ball.rotation, ball.origin, depth);
	}
	if( _car.getTexture() != nullptr ) {
		cc::Vec4f destRect;
		destRect.x = _car.getPosition().x;
		destRect.y = _car.getPosition().y;
		destRect.z = _car.getTexture()->getWidth();
		destRect.w = _car.getTexture()->getHeight();
		_spritebatch.draw(_car.getTexture(), destRect, _car.getRotation(), _car.getOrigin(), 0.0f);
	}
	_spritebatch.end();

	device->present();
}

void SpritesDemo::onUnloadContent() {
	_spritebatch.clean();
}
