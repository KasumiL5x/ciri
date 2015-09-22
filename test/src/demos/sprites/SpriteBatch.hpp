#ifndef __spritebatch__
#define __spritebatch__

#include <memory>
#include <queue>
#include <cc/Mat4.hpp>
#include <cc/Vec2.hpp>
#include <ciri/gfx/IGraphicsDevice.hpp>

_declspec(align(16))
struct SpriteConstants {
	cc::Mat4f projection;
	cc::Vec2f screenSize;
};

struct SpriteBatchItem {
	int vbIndex;
	std::weak_ptr<ciri::ITexture2D> texture;

	SpriteBatchItem()
		: vbIndex(-1) {
	}
};

struct SpriteVertex {
	cc::Vec2f position;
	cc::Vec2f scale;

	SpriteVertex()
		: position(0.0f, 0.0f), scale(1.0f, 1.0f) {
	}
};

class SpriteBatch {
public:
	SpriteBatch();
	~SpriteBatch();

	bool create( const std::shared_ptr<ciri::IGraphicsDevice>& device );
	bool begin( const std::shared_ptr<ciri::IBlendState>& blendState, const std::shared_ptr<ciri::ISamplerState>& samplerState, const std::shared_ptr<ciri::IDepthStencilState>& depthStencilState, const std::shared_ptr<ciri::IRasterizerState>& rasterizerState );
	bool draw( const std::shared_ptr<ciri::ITexture2D>& texture, const cc::Vec2f& position, const cc::Vec2f& scale );
	bool end();
	void clean();

private:
	bool configure();
	int getNextFreeIndex() const;

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

	const unsigned int MAX_SPRITES;
	SpriteVertex* _sprites;
	std::shared_ptr<ciri::IVertexBuffer> _spritesBuffer;

	std::queue<SpriteBatchItem> _batchedItems;
};

#endif /* __spritebatch__ */