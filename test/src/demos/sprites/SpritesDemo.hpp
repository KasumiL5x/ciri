#ifndef __spritesdemo__
#define __spritesdemo__

#include "../IDemo.hpp"
#include "SpriteBatch.hpp"
#include "DynamicGrid.hpp"
#include "PlayerShip.hpp"

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
	SpriteBatch _spritebatch;

	std::shared_ptr<ciri::IBlendState> _blendState;
	std::shared_ptr<ciri::ISamplerState> _samplerState;
	std::shared_ptr<ciri::IDepthStencilState> _depthStencilState;
	std::shared_ptr<ciri::IRasterizerState> _rasterizerState;

	DynamicGrid _grid;

	std::shared_ptr<ciri::ITexture2D> _playerTexture;
	PlayerShip _player;
	cc::Vec2f _playerMovement;
};

#endif /* __spritesdemo__ */
