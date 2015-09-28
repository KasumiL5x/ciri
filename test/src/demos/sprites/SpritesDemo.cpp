#include "SpritesDemo.hpp"
#include <ciri/core/PNG.hpp>
#include <cc/Common.hpp>
#include <ctime>

SpritesDemo::SpritesDemo()
	: IDemo(), _ballsMoving(true) {
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

	// set the ball spawning position
	_ballSpawnPosition.x = 50.0f;
	_ballSpawnPosition.y = static_cast<float>(window()->getHeight()) * 0.5f;
}

void SpritesDemo::onLoadContent() {
	// load textures
	ciri::PNG png;
	if( png.loadFromFile("sprites/textures/ball.png") && (4 == png.getBytesPerPixel()) ) {
		_texture = graphicsDevice()->createTexture2D(png.getWidth(), png.getHeight(), ciri::TextureFormat::Color, 0, png.getPixels());
	}

	// initialize balls
	const int NUM_BALLS = 20;
	for( int i = 0; i < NUM_BALLS; ++i ) {
		Ball b;
		b.setTexture(_texture);
		b.setActive(false);
		_balls.push_back(b);
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
	if( !window()->hasFocus() ) {
		return;
	}
	// check for close w/ escape
	if( input()->isKeyDown(ciri::Key::Escape) ) {
		this->gtfo();
		return;
	}

	// toggle balls update
	if( input()->isKeyDown(ciri::Key::P) && input()->wasKeyUp(ciri::Key::P) ) {
		_ballsMoving = !_ballsMoving;
	}

	// fire!
	if( input()->isMouseButtonDown(ciri::MouseButton::Left) && input()->wasMouseButtonUp(ciri::MouseButton::Left) ) {
		for( auto& b : _balls ) {
			if( b.isActive() ) {
				continue;
			}

			b.setActive(true);
			b.setPosition(_ballSpawnPosition);
			b.setVelocity(cc::Vec2f(1000.0f, 100.0f));
			b.setRotation(0.0f);
			break;
		}
	}
}

void SpritesDemo::onFixedUpdate( double deltaTime, double elapsedTime ) {
	if( _ballsMoving ) {

		for( auto& b : _balls ) {
			if( !b.isActive() ) {
				continue;
			}

			b.setVelocity(b.getVelocity() + cc::Vec2f(0.0f, -9.8f));

			// integrate
			b.setPosition(b.getPosition() + b.getVelocity() * static_cast<float>(deltaTime));

			// disable if off screen (bad hack because i need to go to uni now!)
			if( b.getPosition().x < 0.0f ) {
				b.setActive(false);
			}
			if( b.getPosition().x > window()->getWidth() ) {
				b.setActive(false);
			}
			if( b.getPosition().y < 0.0f ) {
				b.setActive(false);
			}
			if( b.getPosition().y > window()->getHeight() ) {
				b.setActive(false);
			}

		}

		//for( auto& ball : _balls ) {
		//	ball.velocity.y -= 20.8f;
		//	ball.step(deltaTime);
		//	ball.collideWalls(0.0f, window()->getWidth(), 0.0f, window()->getHeight());
		//	ball.rotation += cc::math::degreesToRadians(ball.velocity.x * deltaTime);
		//	ball.rotation = cc::math::wrapAngle(ball.rotation, 0.0f, cc::math::degreesToRadians(360.0f));
		//}
	}
}

void SpritesDemo::onDraw() {
	const std::shared_ptr<ciri::IGraphicsDevice> device = graphicsDevice();

	device->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);
	
	_spritebatch.begin(_blendState, _samplerState, _depthStencilState, _rasterizerState, SpriteSortMode::Deferred);
	int counter = 0;
	for( const auto& ball : _balls ) {
		counter += 1;

		if( !ball.isActive() ) {
			continue;
		}

		const float depth = static_cast<float>(counter) / static_cast<float>(_balls.size());
		_spritebatch.draw(ball.getTexture(), ball.getPosition(), ball.getRotation(), ball.getOrigin(), 1.0f, depth);
	}
	_spritebatch.end();

	device->present();
}

void SpritesDemo::onUnloadContent() {
	_spritebatch.clean();
}
