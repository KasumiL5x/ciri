#ifndef __ciri_game_SpriteBatch__
#define __ciri_game_SpriteBatch__

#include <memory>
#include <vector>
#include <queue>
#include <string>
#include <cc/Mat4.hpp>
#include <cc/Vec2.hpp>
#include <cc/Vec4.hpp>
#include <ciri/Graphics.hpp>
#include "SpriteVertex.hpp"
#include "SpriteBatchItem.hpp"
#include "ISpriteFont.hpp"

namespace ciri {

_declspec(align(16))
struct SpriteConstants {
	cc::Mat4f projection;
};

enum class SpriteSortMode {
	Deferred, // order of user draw calls a.k.a untouched
	Texture, // sort by texture for optimum texture switches
	FrontToBack,
	BackToFront
};

class SpriteBatch {
public:
	SpriteBatch();
	~SpriteBatch();

	bool create( const std::shared_ptr<ciri::IGraphicsDevice>& device );
	bool begin( const std::shared_ptr<ciri::IBlendState>& blendState, const std::shared_ptr<ciri::ISamplerState>& samplerState, const std::shared_ptr<ciri::IDepthStencilState>& depthStencilState, const std::shared_ptr<ciri::IRasterizerState>& rasterizerState, SpriteSortMode sortMode, const std::shared_ptr<ciri::IShader>& shader );

	/**
	 * Draws a sprite.
	 * @param texture  Texture to draw.
	 * @param dstRect  X and Y are the position on the screen in pixels; Z and W are the width and height on the screen in pixels.
	 * @param rotation Rotation angle in radians.
	 * @param origin   Pivot point where {0, 0} is the bottom left.
	 * @param depth     Depth used for rendering and sorting of sprites.
	 */
	void draw( const std::shared_ptr<ciri::ITexture2D>& texture, const cc::Vec4f& dstRect, float rotation, const cc::Vec2f& origin, float depth );

	/**
	 * Draws a sprite.
	 * @param texture   Texture to draw.
	 * @param position  X and Y are the position on the screen in pixels.
	 * @param rotation  Rotation angle in radians.
	 * @param origin    Pivot point where {0, 0} is the bottom left.
	 * @param scale     Scaling factor.
	 * @param depth     Depth used for rendering and sorting of sprites.
	 */
	void draw( const std::shared_ptr<ciri::ITexture2D>& texture, const cc::Vec2f& position, float rotation, const cc::Vec2f& origin, const cc::Vec2f& scale, float depth );

	/**
	 * Draws a sprite.
	 * @param texture   Texture to draw.
	 * @param position  X and Y are the position on the screen in pixels.
	 * @param rotation  Rotation angle in radians.
	 * @param origin    Pivot point where {0, 0} is the bottom left.
	 * @param scale     Uniform scaling factor.
	 * @param depth     Depth used for rendering and sorting of sprites.
	 */
	void draw( const std::shared_ptr<ciri::ITexture2D>& texture, const cc::Vec2f& position, float rotation, const cc::Vec2f& origin, float scale, float depth );

	/**
	 * Draws a sprite.
	 * @param texture   Texture to draw.
	 * @param position  X and Y are the position on the screen in pixels.
	 * @param rotation  Rotation angle in radians.
	 * @param origin    Pivot point where {0, 0} is the bottom left.
	 * @param scale     Scaling factor.
	 * @param depth     Depth used for rendering and sorting of sprites.
	 * @param color     Color (including alpha) to multiply the texture by.
	 */
	void draw( const std::shared_ptr<ciri::ITexture2D>& texture, const cc::Vec2f& position, float rotation, const cc::Vec2f& origin, const cc::Vec2f& scale, float depth, const cc::Vec4f& color );


	void drawString( const std::string& text, const std::shared_ptr<ISpriteFont>& font, const cc::Vec2f& position, const cc::Vec4f& color );
	
	bool end();
	void clean();

private:
	bool configure();
	std::shared_ptr<SpriteBatchItem> createBatchItem();
	void ensureArrayCapacity( int size );
	void flush( int start, int end, const std::shared_ptr<ciri::ITexture2D>& texture );

private:
	std::shared_ptr<ciri::IGraphicsDevice> _device; // external

	std::shared_ptr<ciri::IBlendState> _blendState; // external
	std::shared_ptr<ciri::ISamplerState> _samplerState; // external
	std::shared_ptr<ciri::IDepthStencilState> _depthStencilState; // external
	std::shared_ptr<ciri::IRasterizerState> _rasterizerState; // external
	std::shared_ptr<ciri::IShader> _defaultShader; // default sprite shader loaded within the spritebatch
	std::shared_ptr<ciri::IShader> _shader; // shader to actually use for drawing (can be external or internal)

	SpriteConstants _constants;
	std::shared_ptr<ciri::IConstantBuffer> _constantBuffer;

	bool _beginCalled;

	std::shared_ptr<ciri::IVertexBuffer> _spritesBuffer;

	std::vector<std::shared_ptr<SpriteBatchItem>> _batchItemList;
	std::queue<std::shared_ptr<SpriteBatchItem>> _freeBatchItemQueue;

	//std::vector<SpriteVertex> _vertexArray;
	SpriteVertex* _vertexArray;
	int _vertexArraySize;

	SpriteSortMode _sortMode;
};

}

#endif