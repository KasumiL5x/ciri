#ifndef __spritesdemo__
#define __spritesdemo__

#include <array>
#include <memory>
#include <ciri/Game.hpp>
#include "../../common/SpriteBatch.hpp"
#include "PlayerShip.hpp"
#include "Bullet.hpp"
#include <cc/Quaternion.hpp>
#include "Enemy.hpp"
#include "TestParticleSystem.hpp"

#include "BMGrid.hpp"

class SpritesDemo : public ciri::App {
public:
	SpritesDemo();
	virtual ~SpritesDemo();
	void* operator new( size_t i );
	void operator delete( void* p );

	virtual void onInitialize() override;
	virtual void onLoadContent() override;
	virtual void onEvent( const ciri::WindowEvent& evt ) override;
	virtual void onUpdate( const double deltaTime, const double elapsedTime ) override;
	virtual void onFixedUpdate( const double deltaTime, const double elapsedTime ) override;
	virtual void onDraw() override;
	virtual void onUnloadContent() override;

private:
	void addBullet( const cc::Vec2f& position, const cc::Vec2f& velocity );
	bool isColliding( const Entity& a, const Entity& b ) const;
	bool spawnEnemy();

private:
	SpriteBatch _spritebatch;

	std::shared_ptr<ciri::IBlendState> _blendState;
	std::shared_ptr<ciri::ISamplerState> _samplerState;
	std::shared_ptr<ciri::IDepthStencilState> _depthStencilState;
	std::shared_ptr<ciri::IRasterizerState> _rasterizerState;

	BMGrid* _grid;

	std::shared_ptr<ciri::ITexture2D> _playerTexture;
	std::shared_ptr<PlayerShip> _player;
	cc::Vec2f _playerMovement;

	std::shared_ptr<ciri::ITexture2D> _bulletTexture;
	std::array<Bullet, 100> _bullets;
	float _fireTimer = {0.0f};
	const float FIRE_DELAY = {0.1f};

	std::shared_ptr<ciri::ITexture2D> _enemySeekerTexture;
	std::array<Enemy, 10> _enemies;
	float _enemySpawnDelay;
	float _enemySpawnTimer;

	std::shared_ptr<ciri::ITexture2D> _cursorTexture;
	cc::Vec2f _cursorPos;
	cc::Vec2f _cursorOrigin;

	TestParticleSystem _psys;
	std::shared_ptr<ciri::ITexture2D> _testPsysTexture;
};

#endif /* __spritesdemo__ */
