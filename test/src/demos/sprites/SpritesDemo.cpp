#include "SpritesDemo.hpp"
#include <ciri/core/PNG.hpp>
#include <cc/Common.hpp>
#include <ctime>
#include <cc/Quaternion.hpp>

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

	window()->setCursorVisible(false);

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
	_player = std::make_shared<PlayerShip>();
	_player->setPosition(cc::Vec2f(vp.width() * 0.5f, vp.height() * 0.5f));
	_grid.applyDirectedForce(cc::Vec3f(0.0f, 0.0f, 5000.0f), cc::Vec3f(_player->getPosition().x, _player->getPosition().y, 0.0f), 50.0f);
}

void SpritesDemo::onLoadContent() {
	// load and set player texture
	ciri::PNG playerPng;
	if( playerPng.loadFromFile("sprites/textures/Player.png") && (4 == playerPng.getBytesPerPixel()) ) {
		_playerTexture = graphicsDevice()->createTexture2D(playerPng.getWidth(), playerPng.getHeight(), ciri::TextureFormat::Color, 0, playerPng.getPixels());
	}
	_player->setTexture(_playerTexture);

	// load bullet texture
	ciri::PNG bulletPng;
	if( bulletPng.loadFromFile("sprites/textures/Bullet.png") && (4 == bulletPng.getBytesPerPixel()) ) {
		_bulletTexture = graphicsDevice()->createTexture2D(bulletPng.getWidth(), bulletPng.getHeight(), ciri::TextureFormat::Color, 0, bulletPng.getPixels());
	}
	// assign bullet textures
	for( auto& bullet : _bullets ) {
		bullet.setTexture(_bulletTexture);
	}

	// load enemy textures
	ciri::PNG seekPng;
	if( seekPng.loadFromFile("sprites/textures/Seeker.png") && (4 == seekPng.getBytesPerPixel()) ) {
		_enemySeekerTexture = graphicsDevice()->createTexture2D(seekPng.getWidth(), seekPng.getHeight(), ciri::TextureFormat::Color, 0, seekPng.getPixels());
	}

	// load some enemies
	for( int i = 0; i < _enemies.size(); ++i ) {
		const float x = cc::math::randRange<float>(0.0f, static_cast<float>(window()->getWidth()));
		const float y = cc::math::randRange<float>(0.0f, static_cast<float>(window()->getHeight()));
		_enemies[i] = Enemy::createSeeker(cc::Vec2f(x, y));
		_enemies[i].setTexture(_enemySeekerTexture);
		_enemies[i].setIsAlive(true);
		_enemies[i].setTarget(_player);
	}

	// custom cursor texture
	ciri::PNG cursorPng;
	if( cursorPng.loadFromFile("sprites/textures/Pointer.png") && (4 == cursorPng.getBytesPerPixel()) ) {
		_cursorTexture = graphicsDevice()->createTexture2D(cursorPng.getWidth(), cursorPng.getHeight(), ciri::TextureFormat::Color, 0, cursorPng.getPixels());
		_cursorOrigin = cc::Vec2f(0.0f, _cursorTexture->getHeight());
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

	// move custom cursor
	if( _cursorTexture != nullptr ) {
		_cursorPos = cc::Vec2f(static_cast<float>(input()->mouseX()), static_cast<float>(window()->getHeight() - input()->mouseY()));
		_cursorPos.x = (_cursorPos.x < 0.0f) ? 0.0f : _cursorPos.x;
		_cursorPos.y = (_cursorPos.y < _cursorTexture->getHeight()) ? _cursorTexture->getHeight() : _cursorPos.y;
		_cursorPos.x = (_cursorPos.x > window()->getWidth() - _cursorTexture->getWidth()) ? window()->getWidth() - _cursorTexture->getWidth() : _cursorPos.x;
		_cursorPos.y = (_cursorPos.y > window()->getHeight()) ? window()->getHeight() : _cursorPos.y;

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

	_fireTimer += deltaTime;

	// firing
	if( input()->isMouseButtonDown(ciri::MouseButton::Left) && (_fireTimer > FIRE_DELAY) ) {
		_fireTimer = 0.0f;

		const cc::Vec2f mousePos(static_cast<float>(input()->mouseX()), static_cast<float>(window()->getHeight() - input()->mouseY()));
		const cc::Vec2f diff = (mousePos - _player->getPosition()).normalized();

		const float aimAngle = atan2f(diff.y, diff.x);
		const cc::Quatf aimQuat = quatYawPitchRoll(0.0f, 0.0f, aimAngle);

		const float randomSpread = cc::math::randRange<float>(-0.04f, 0.04f) + cc::math::randRange<float>(-0.04f, 0.04f);
		const cc::Vec2f vel = fromPolar(aimAngle + randomSpread, 11.0f);

		const cc::Vec2f offset1 = transformVec2Quat(cc::Vec2f(35.0f, -8.0f), aimQuat);
		addBullet(_player->getPosition() + offset1, vel);

		const cc::Vec2f offset2 = transformVec2Quat(cc::Vec2f(35.0f, 8.0f), aimQuat);
		addBullet(_player->getPosition() + offset2, vel);
	}

	if( input()->isMouseButtonDown(ciri::MouseButton::Right) ) {
		_grid.applyDirectedForce(cc::Vec3f(0.0f, 0.0f, 5000.0f), cc::Vec3f(_player->getPosition().x, _player->getPosition().y, 0.0f), 50.0f);
		//_grid.applyExplosiveForce()
	}
}

void SpritesDemo::onFixedUpdate( double deltaTime, double elapsedTime ) {
	_player->update(_playerMovement);

	const ciri::Viewport& vp = graphicsDevice()->getViewport();
	const cc::Vec4f bounds(static_cast<float>(vp.x()), static_cast<float>(vp.y()), static_cast<float>(vp.width()), static_cast<float>(vp.height()));
	for( auto& curr : _bullets ) {
		if( !curr.isAlive() ) {
			continue;
		}
		curr.update(bounds);
		_grid.applyExplosiveForce(0.5f * curr.getVelocity().magnitude(), curr.getPosition(), 80.0f);
	}

	const cc::Vec2f screenSize = cc::Vec2f(static_cast<float>(vp.width()), static_cast<float>(vp.height()));
	for( auto& curr : _enemies ) {
		if( !curr.isAlive() ) {
			return;
		}
		curr.update(screenSize);
	}

	_grid.update();
}

void SpritesDemo::onDraw() {
	const std::shared_ptr<ciri::IGraphicsDevice> device = graphicsDevice();
	const ciri::Viewport& vp = device->getViewport();

	device->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);

	_spritebatch.begin(_blendState, _samplerState, _depthStencilState, _rasterizerState, SpriteSortMode::Deferred, nullptr);

	// grid
	_grid.draw(_spritebatch, static_cast<float>(vp.width()), static_cast<float>(vp.height()));

	// enemies
	for( auto& curr : _enemies ) {
		if( !curr.isAlive() ) {
			continue;
		}
		_spritebatch.draw(curr.getTexture(), curr.getPosition(), curr.getOrientation(), curr.getOrigin(), 1.0f, 1.0f);
	}

	// player
	_spritebatch.draw(_player->getTexture(), _player->getPosition(), _player->getOrientation(), _player->getOrigin(), 1.0f, 1.0f);

	// bullets
	for( auto& bullet : _bullets ) {
		if( !bullet.isAlive() ) {
			continue;
		}
		_spritebatch.draw(bullet.getTexture(), bullet.getPosition(), bullet.getOrientation(), bullet.getOrigin(), 1.0f, 1.0f);
	}
	
	// cursor
	if( _cursorTexture != nullptr ) {
		_spritebatch.draw(_cursorTexture, _cursorPos, 0.0f, _cursorOrigin, 1.0f, 1.0f);
	}


	_spritebatch.end();

	device->present();
}

void SpritesDemo::onUnloadContent() {
	_spritebatch.clean();
}

void SpritesDemo::addBullet( const cc::Vec2f& position, const cc::Vec2f& velocity ) {
	for( auto& curr : _bullets ) {
		if( curr.isAlive() ) {
			continue;
		}

		curr.setIsAlive(true);
		curr.setPosition(position);
		curr.setVelocity(velocity);
		break;
	}
}

cc::Vec2f SpritesDemo::fromPolar( float angle, float magnitude ) const {
	return magnitude * cc::Vec2f(cosf(angle), sinf(angle));
}

cc::Vec2f SpritesDemo::transformVec2Quat( const cc::Vec2f& value, const cc::Quatf& rotation ) const {
	const cc::Vec3f rot1 = cc::Vec3f(rotation.x + rotation.x, rotation.y + rotation.y, rotation.z + rotation.z);
	const cc::Vec3f rot2 = cc::Vec3f(rotation.x, rotation.x, rotation.w);
	const cc::Vec3f rot3 = cc::Vec3f(1, rotation.y, rotation.z);
	const cc::Vec3f rot4 = rot1*rot2;
	const cc::Vec3f rot5 = rot1*rot3;

	cc::Vec2f v;
  v.x = (float)((double)value.x * (1.0 - (double)rot5.y - (double)rot5.z) + (double)value.y * ((double)rot4.y - (double)rot4.z));
  v.y = (float)((double)value.x * ((double)rot4.y + (double)rot4.z) + (double)value.y * (1.0 - (double)rot4.x - (double)rot5.z));
	return v;
}

cc::Quatf SpritesDemo::quatYawPitchRoll( float yaw, float pitch, float roll ) const {
	cc::Quatf quaternion;
	const float num9 = roll * 0.5f;
	const float num6 = sinf(num9);
	const float num5 = cosf(num9);
	const float num8 = pitch * 0.5f;
	const float num4 = sinf(num8);
	const float num3 = cosf(num8);
	const float num7 = yaw * 0.5f;
	const float num2 = sinf(num7);
	const float num = cosf(num7);
	quaternion.x = ((num * num4) * num5) + ((num2 * num3) * num6);
	quaternion.y = ((num2 * num3) * num5) - ((num * num4) * num6);
	quaternion.z = ((num * num3) * num6) - ((num2 * num4) * num5);
	quaternion.w = ((num * num3) * num5) + ((num2 * num4) * num6);
	return quaternion;
}