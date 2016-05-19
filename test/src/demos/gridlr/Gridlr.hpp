#ifndef __gridlr__
#define __gridlr__

#include <memory>
#include "../../common/Game.hpp"
#include "../../common/SpriteBatch.hpp"
#include "BlockGrid.hpp"

class Gridlr : public Game {
public:
	Gridlr();
	virtual ~Gridlr();

protected:
	virtual void onInitialize();
	virtual void onLoadContent();
	virtual void onEvent(const ciri::core::WindowEvent& evt);
	virtual void onUpdate(const double deltaTime, const double elapsedTime);
	virtual void onFixedUpdate(const double deltaTime, const double elapsedTime);
	virtual void onDraw();
	virtual void onUnloadContent();

private:
	std::unique_ptr<gridlr::BlockGrid> _grid;
	SpriteBatch _spriteBatch;
	std::shared_ptr<ciri::graphics::IBlendState> _blendState;
	std::shared_ptr<ciri::graphics::ISamplerState> _samplerState;
	std::shared_ptr<ciri::graphics::IDepthStencilState> _depthStencilState;
	std::shared_ptr<ciri::graphics::IRasterizerState> _rasterizerState;
	std::shared_ptr<ciri::graphics::ITexture2D> _cellTexture;
	cc::Vec2f _gridOffset;
	bool _isDragging;
	cc::Vec2i _lastMouseCell;
	//
	std::shared_ptr<ciri::graphics::ITexture2D> _redTextureTest;
	std::shared_ptr<ciri::graphics::IShader> _redShader;
	std::shared_ptr<ciri::graphics::IVertexBuffer> _redVb;
	std::shared_ptr<ciri::graphics::ISamplerState> _redSampler;
};

#endif /* __gridlr__ */