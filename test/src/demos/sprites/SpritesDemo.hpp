#ifndef __spritesdemo__
#define __spritesdemo__

#include "../IDemo.hpp"
#include "SpriteBatch.hpp"
#include <cc/Vec2.hpp>

struct Ball {
	std::shared_ptr<ciri::ITexture2D> texture;
	cc::Vec2f position;
	cc::Vec2f velocity;
	float rotation;
	cc::Vec2f origin;

	void step( float deltaTime ) {
		position += velocity * deltaTime;
	}

	void collideWalls( float left, float right, float top, float bottom ) {
		bool collided = false;

		if( (position.x - origin.x) < left ) {
			position.x = left + origin.x;
			velocity.x = -velocity.x;
			collided = true;
		}

		if( (position.x - origin.x) > (right - texture->getWidth()) ) {
			position.x = (right - texture->getWidth()) + origin.x;
			velocity.x = -velocity.x;
			collided = true;
		}

		if( (position.y - origin.y) < top ) {
			position.y = top + origin.y;
			velocity.y = -velocity.y;
			collided = true;
		}

		if( (position.y - origin.y) > (bottom - texture->getHeight()) ) {
			position.y = (bottom - texture->getHeight()) + origin.y;
			velocity.y = -velocity.y;
			collided = true;
		}

		if( collided ) {
			velocity *= 0.9f;
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
	bool _ballsMoving;
};

#endif /* __spritesdemo__ */
