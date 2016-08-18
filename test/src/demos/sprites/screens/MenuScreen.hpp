#ifndef __test_sprites_MenuScreen__
#define __test_sprites_MenuScreen__

#include <memory>
#include <ciri/Game.hpp>
#include <ciri/Graphics.hpp>

class MenuScreen : public ciri::Screen {
public:
	MenuScreen( const std::shared_ptr<ciri::IGraphicsDevice>& device, const std::shared_ptr<ciri::SpriteBatch>& spriteBatch, const std::shared_ptr<ciri::IInput>& input );
	virtual ~MenuScreen();

	virtual void onAdd() override;
	virtual void onRemove() override;
	virtual void onUpdate( double deltaTime, double elapsedTime ) override;
	virtual void onFixedUpdate( double deltaTime, double elapsedTime ) override;
	virtual void onDraw() override;
	virtual int zIndex() const override;

private:
	std::shared_ptr<ciri::IGraphicsDevice> _device;
	std::shared_ptr<ciri::SpriteBatch> _spriteBatch;
	std::shared_ptr<ciri::IInput> _input;
	std::shared_ptr<ciri::IBlendState> _blendState;
	std::shared_ptr<ciri::ISamplerState> _samplerState;
	std::shared_ptr<ciri::IDepthStencilState> _depthStencilState;
	std::shared_ptr<ciri::IRasterizerState> _rasterizerState;
	std::shared_ptr<ciri::ISpriteFont> _titleFont;
	std::shared_ptr<ciri::ISpriteFont> _menuFont;
	int _selectedMenuIndex;
};

#endif