#ifndef __spritebatch__
#define __spritebatch__

#include <memory>
#include <vector>
#include <queue>
#include <cc/Mat4.hpp>
#include <cc/Vec2.hpp>
#include <cc/Vec4.hpp>
#include <ciri/Graphics.hpp>
#include "SpriteVertex.hpp"
#include "SpriteBatchItem.hpp"

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
	bool begin( const std::shared_ptr<ciri::IBlendState>& blendState, const std::shared_ptr<ciri::ISamplerState>& samplerState, const std::shared_ptr<ciri::IDepthStencilState>& depthStencilState, const std::shared_ptr<ciri::IRasterizerState>& rasterizerState, SpriteSortMode sortMode );

	/**
	 * Draws a sprite with a texture, position, width, height, rotation, and pivot origin.
	 * @param texture  Texture to draw.
	 * @param dstRect  X and Y are the position on the screen in pixels; Z and W are the width and height on the screen in pixels.
	 * @param rotation Rotation angle in radians.
	 * @param origin   Pivot point where {0, 0} is the bottom left.
	 */
	void draw( const std::shared_ptr<ciri::ITexture2D>& texture, const cc::Vec4f& dstRect, float rotation, const cc::Vec2f& origin, float depth );
	
	bool end();
	void clean();

	void debugReloadShaders();

private:
	bool configure();
	std::shared_ptr<SpriteBatchItem> createBatchItem();
	void ensureArrayCapacity( int size );

private:
	std::shared_ptr<ciri::IGraphicsDevice> _device; // external

	std::shared_ptr<ciri::IBlendState> _blendState; // external
	std::shared_ptr<ciri::ISamplerState> _samplerState; // external
	std::shared_ptr<ciri::IDepthStencilState> _depthStencilState; // external
	std::shared_ptr<ciri::IRasterizerState> _rasterizerState; // external
	std::shared_ptr<ciri::IShader> _shader;

	SpriteConstants _constants;
	std::shared_ptr<ciri::IConstantBuffer> _constantBuffer;

	bool _beginCalled;

	std::shared_ptr<ciri::IVertexBuffer> _spritesBuffer;

	std::vector<std::shared_ptr<SpriteBatchItem>> _batchItemList;
	std::queue<std::shared_ptr<SpriteBatchItem>> _freeBatchItemQueue;

	std::vector<SpriteVertex> _vertexArray;

	SpriteSortMode _sortMode;
};

#endif /* __spritebatch__ */