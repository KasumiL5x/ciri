#ifndef __spritesdemo__
#define __spritesdemo__

#include "../IDemo.hpp"
#include "SpriteBatch.hpp"
#include <cc/Vec2.hpp>

struct Ball {
	std::shared_ptr<ciri::ITexture2D> texture;
	cc::Vec2f position;
	cc::Vec2f velocity;

	void step( float deltaTime ) {
		position += velocity * deltaTime;
	}

	void collideWalls( float left, float right, float top, float bottom ) {
		if( position.x < left ) {
			position.x = left;
			velocity.x = -velocity.x;
		}

		if( position.x > (right - texture->getWidth()) ) {
			position.x = (right - texture->getWidth());
			velocity.x = -velocity.x;
		}

		if( position.y < top ) {
			position.y = top;
			velocity.y = -velocity.y;
		}

		if( position.y > (bottom - texture->getHeight()) ) {
			position.y = (bottom - texture->getHeight());
			velocity.y = -velocity.y;
		}
	}
};

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
	virtual void onDraw() override;
	virtual void onUnloadContent() override;

private:
	SpriteBatch _spritebatch;

	std::shared_ptr<ciri::IBlendState> _blendState;
	std::shared_ptr<ciri::ISamplerState> _samplerState;
	std::shared_ptr<ciri::IDepthStencilState> _depthStencilState;
	std::shared_ptr<ciri::IRasterizerState> _rasterizerState;

	std::shared_ptr<ciri::ITexture2D> _texture;

	std::vector<Ball> _balls;
};

#endif /* __spritesdemo__ */
