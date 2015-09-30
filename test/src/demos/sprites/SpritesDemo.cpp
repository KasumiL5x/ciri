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

	const ciri::Viewport& vp = graphicsDevice()->getViewport();
	const int maxGridPoints = 1600;
	const cc::Vec2f gridSpacing = cc::Vec2f(sqrtf(vp.width() * vp.height() / maxGridPoints));
	_grid = new DynamicGrid(cc::Vec4f(vp.x(), vp.y(), vp.width(), vp.height()), gridSpacing, graphicsDevice());
	_grid->applyImplosiveForce(1000.0f, cc::Vec2f(vp.width()/2, vp.height()/2), 100.0f);
	//_grid->applyDirectedForce(cc::Vec2f(100.0f, 100.0f), cc::Vec2f(vp.width()/2, vp.height()/2), 10.0f);
}

void SpritesDemo::onLoadContent() {
	// load background texture
	ciri::PNG bgPng;
	if( bgPng.loadFromFile("sprites/textures/bg.png") && (4 == bgPng.getBytesPerPixel()) ) {
		_backgroundTexture = graphicsDevice()->createTexture2D(bgPng.getWidth(), bgPng.getHeight(), ciri::TextureFormat::Color, 0, bgPng.getPixels());
	}

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
		const cc::Vec2f curr = cc::Vec2f(input()->mouseX(), window()->getHeight()-input()->mouseY());
		//_grid->applyExplosiveForce(50.0f, curr, 100.0f);
		for( auto& b : _balls ) {
			if( b.isActive() ) {
				continue;
			}

			const cc::Vec2f mousePosition = cc::Vec2f(static_cast<float>(input()->mouseX()), window()->getHeight() - static_cast<float>(input()->mouseY()));
			const cc::Vec2f direction = (mousePosition - _ballSpawnPosition).normalized();

			const float SPEED = 1250.0f;

			b.setActive(true);
			b.setPosition(_ballSpawnPosition);
			b.setVelocity(direction * SPEED);
			b.setRotation(0.0f);
			break;
		}
	}

	// mouse grid effect
	if( input()->mouseX() != input()->lastMouseX() || input()->mouseY() != input()->lastMouseY() ) {
		const cc::Vec2f curr = cc::Vec2f(input()->mouseX(), window()->getHeight()-input()->mouseY());
		const cc::Vec2f prev = cc::Vec2f(input()->lastMouseX(), window()->getHeight()-input()->lastMouseY());
		const cc::Vec2f diff = curr - prev;
		//_grid->applyDirectedForce(diff, curr, 10.0f);
	}

	for( auto& b : _balls ) {
		if( !b.isActive() ) {
			continue;
		}
		_grid->applyDirectedForce(b.getVelocity() * 0.01f, b.getPosition(), 10.0f);
		//_grid->applyExplosiveForce(100.0f, pos, 10.0f);
		//_grid->applyImplosiveForce(100.0f, pos, 10.0f);
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

		_grid->update();
	}
}

void SpritesDemo::onDraw() {
	const std::shared_ptr<ciri::IGraphicsDevice> device = graphicsDevice();

	device->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);
	
	_spritebatch.begin(_blendState, _samplerState, _depthStencilState, _rasterizerState, SpriteSortMode::Deferred, nullptr);
	//_spritebatch.draw(_backgroundTexture, cc::Vec2f(0.0f, 0.0f), 0.0f, cc::Vec2f(0.0f, 0.0f), 1.0f, 0.0f);

	const ciri::Viewport& vp = device->getViewport();
	_grid->draw(_spritebatch, vp.width(), vp.height());

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
	if( _grid != nullptr ) {
		delete _grid; _grid = nullptr;
	}
	_spritebatch.clean();
}
