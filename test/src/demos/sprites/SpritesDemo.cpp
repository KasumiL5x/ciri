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
	srand(static_cast<unsigned int>(time(0)));
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
	_blendState = graphicsDevice()->getDefaultBlendAlpha();
	_samplerState = graphicsDevice()->createSamplerState(ciri::SamplerDesc());
	_depthStencilState = graphicsDevice()->getDefaultDepthStencilNone();
	_rasterizerState = graphicsDevice()->getDefaultRasterCounterClockwise();

	// configure grid
	const ciri::Viewport& vp = graphicsDevice()->getViewport();
	const int maxGridPoints = 1600;
	const cc::Vec2f gridSpacing = cc::Vec2f(sqrtf(static_cast<float>(vp.width() * vp.height() / maxGridPoints)));
	_grid = DynamicGrid(cc::Vec4f(static_cast<float>(vp.x()), static_cast<float>(vp.y()), static_cast<float>(vp.width()), static_cast<float>(vp.height())), gridSpacing, graphicsDevice());

	// configure player and set off initial spawn explosion
	_player.setPosition(cc::Vec2f(vp.width() * 0.5f, vp.height() * 0.5f));
	_grid.applyDirectedForce(cc::Vec3f(0.0f, 0.0f, 5000.0f), cc::Vec3f(_player.getPosition().x, _player.getPosition().y, 0.0f), 50.0f);
}

void SpritesDemo::onLoadContent() {
	// load and set player texture
	ciri::PNG playerPng;
	if( playerPng.loadFromFile("sprites/textures/Player.png") && (4 == playerPng.getBytesPerPixel()) ) {
		_playerTexture = graphicsDevice()->createTexture2D(playerPng.getWidth(), playerPng.getHeight(), ciri::TextureFormat::Color, 0, playerPng.getPixels());
	}
	_player.setTexture(_playerTexture);
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

	// update player movement
	_playerMovement = cc::Vec2f::zero();
	if( input()->isKeyDown(ciri::Key::A) ) {
		_playerMovement.x -= 1.0f;
	}
	if( input()->isKeyDown(ciri::Key::D) ) {
		_playerMovement.x += 1.0f;
	}
	if( input()->isKeyDown(ciri::Key::W) ) {
		_playerMovement.y += 1.0f;
	}
	if( input()->isKeyDown(ciri::Key::S) ) {
		_playerMovement.y -= 1.0f;
	}
	if( _playerMovement.sqrMagnitude() > 1.0f ) {
		_playerMovement.normalize();
	}
}

void SpritesDemo::onFixedUpdate( double deltaTime, double elapsedTime ) {
	_player.update(_playerMovement);
	_grid.update();
}

void SpritesDemo::onDraw() {
	const std::shared_ptr<ciri::IGraphicsDevice> device = graphicsDevice();
	const ciri::Viewport& vp = device->getViewport();

	device->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);

	_spritebatch.begin(_blendState, _samplerState, _depthStencilState, _rasterizerState, SpriteSortMode::Deferred, nullptr);
	_grid.draw(_spritebatch, static_cast<float>(vp.width()), static_cast<float>(vp.height()));
	_spritebatch.draw(_player.getTexture(), _player.getPosition(), _player.getOrientation(), _player.getOrigin(), 1.0f, 1.0f);
	_spritebatch.end();

	device->present();
}

void SpritesDemo::onUnloadContent() {
	_spritebatch.clean();
}
