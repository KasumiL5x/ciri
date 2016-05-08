#ifndef __gridlr__
#define __gridlr__

#include <memory>
#include "../../framework/Game.hpp"
#include "../../common/SpriteBatch.hpp"
#include "BlockGrid.hpp"

class Gridlr : public Game {
public:
	Gridlr();
	virtual ~Gridlr();

protected:
	virtual void onInitialize();
	virtual void onLoadContent();
	virtual void onEvent(const ciri::WindowEvent& evt);
	virtual void onUpdate(const double deltaTime, const double elapsedTime);
	virtual void onFixedUpdate(const double deltaTime, const double elapsedTime);
	virtual void onDraw();
	virtual void onUnloadContent();

private:
	std::unique_ptr<gridlr::BlockGrid> _grid;
	SpriteBatch _spriteBatch;
	std::shared_ptr<ciri::IBlendState> _blendState;
	std::shared_ptr<ciri::ISamplerState> _samplerState;
	std::shared_ptr<ciri::IDepthStencilState> _depthStencilState;
	std::shared_ptr<ciri::IRasterizerState> _rasterizerState;
	std::shared_ptr<ciri::ITexture2D> _cellTexture;
	cc::Vec2f _gridOffset;
	bool _isDragging;
	cc::Vec2i _lastMouseCell;
};

#endif /* __gridlr__ */