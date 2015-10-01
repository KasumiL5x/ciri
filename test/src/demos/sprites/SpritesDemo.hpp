#ifndef __spritesdemo__
#define __spritesdemo__

#include <array>
#include <memory>
#include "../IDemo.hpp"
#include "SpriteBatch.hpp"
#include "DynamicGrid.hpp"
#include "PlayerShip.hpp"
#include "Bullet.hpp"
#include <cc/Quaternion.hpp>
#include "Enemy.hpp"

class SpritesDemo : public IDemo {
public:
	SpritesDemo();
	virtual ~SpritesDemo();
	void* operator new( size_t i );
	void operator delete( void* p );

	virtual DemoConfig getConfig() override;
	virtual void onInitialize() override;
	virtual void onLoadContent() override;
	virtual void onEvent( ciri::WindowEvent evt ) override;
	virtual void onUpdate( double deltaTime, double elapsedTime ) override;
	virtual void onFixedUpdate( double deltaTime, double elapsedTime ) override;
	virtual void onDraw() override;
	virtual void onUnloadContent() override;

private:
	void addBullet( const cc::Vec2f& position, const cc::Vec2f& velocity );
	cc::Vec2f fromPolar( float angle, float magnitude ) const;
	cc::Vec2f transformVec2Quat( const cc::Vec2f& value, const cc::Quatf& rotation ) const;
	cc::Quatf quatYawPitchRoll( float yaw, float pitch, float roll ) const;

private:
	SpriteBatch _spritebatch;

	std::shared_ptr<ciri::IBlendState> _blendState;
	std::shared_ptr<ciri::ISamplerState> _samplerState;
	std::shared_ptr<ciri::IDepthStencilState> _depthStencilState;
	std::shared_ptr<ciri::IRasterizerState> _rasterizerState;

	DynamicGrid _grid;

	std::shared_ptr<ciri::ITexture2D> _playerTexture;
	std::shared_ptr<PlayerShip> _player;
	cc::Vec2f _playerMovement;

	std::shared_ptr<ciri::ITexture2D> _bulletTexture;
	std::array<Bullet, 100> _bullets;
	float _fireTimer = {0.0f};
	const float FIRE_DELAY = {0.1f};

	std::shared_ptr<ciri::ITexture2D> _enemySeekerTexture;
	std::array<Enemy, 10> _enemies;
};

#endif /* __spritesdemo__ */
