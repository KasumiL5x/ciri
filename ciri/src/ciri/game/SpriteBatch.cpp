#include <ciri/game/SpriteBatch.hpp>
#include <cc/MatrixFunc.hpp>

using namespace ciri;

struct SpriteSortTexture {
	inline bool operator()( const std::shared_ptr<SpriteBatchItem>& lhs, const std::shared_ptr<SpriteBatchItem>& rhs ) {
		return (lhs->texture == rhs->texture);
	}
};

struct SpriteSortFrontToBack {
	inline bool operator()( const std::shared_ptr<SpriteBatchItem>& lhs, const std::shared_ptr<SpriteBatchItem>& rhs ) {
		return (lhs->depth < rhs->depth);
	}
};

struct SpriteSortBackToFront {
	inline bool operator()( const std::shared_ptr<SpriteBatchItem>& lhs, const std::shared_ptr<SpriteBatchItem>& rhs ) {
		return (rhs->depth < lhs->depth);
	}
};

SpriteBatch::SpriteBatch()
	: _beginCalled(false), _sortMode(SpriteSortMode::Deferred), _vertexArray(nullptr), _vertexArraySize(0) {
}

SpriteBatch::~SpriteBatch() {
	if( _vertexArray != nullptr ) {
		delete[] _vertexArray;
		_vertexArray = nullptr;
	}
}

bool SpriteBatch::create( const std::shared_ptr<ciri::IGraphicsDevice>& device ) {
	// store gfx device
	_device = device;

	_spritesBuffer = device->createVertexBuffer();

	//ensureArrayCapacity(10);

	// load and configure shader and constants
	_defaultShader = device->createShader();
	_defaultShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Position, 0));
	_defaultShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float2, ciri::VertexUsage::Texcoord, 0));
	_defaultShader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float4, ciri::VertexUsage::Color, 0));
	const std::string shaderExt = device->getShaderExt();
	const std::string vsFile = ("data/shaders/SpriteBatch_vs" + shaderExt);
	//const std::string gsFile = ("data/shaders/SpriteBatch_gs" + shaderExt);
	const std::string psFile = ("data/shaders/SpriteBatch_ps" + shaderExt);
	if( ciri::failed(_defaultShader->loadFromFile(vsFile.c_str(), nullptr, psFile.c_str())) ) {
		printf("Failed to load SpriteBatch shader:\n");
		for( auto err : _defaultShader->getErrors() ) {
			printf("%s\n", err.msg.c_str());
		}
		return false;
	}
	_constantBuffer = device->createConstantBuffer();
	if( ciri::failed(_constantBuffer->setData(sizeof(SpriteConstants), &_constants)) ) {
		printf("Failed to create SpriteBatch constants.\n");
		return false;
	}
	if( ciri::failed(_defaultShader->addConstants(_constantBuffer, "SpriteConstants", ciri::ShaderStage::Vertex)) ) {
		printf("Failed to assign constants to SpriteBatch shader.\n");
		return false;
	}

	return true;
}

bool SpriteBatch::begin( const std::shared_ptr<ciri::IBlendState>& blendState, const std::shared_ptr<ciri::ISamplerState>& samplerState, const std::shared_ptr<ciri::IDepthStencilState>& depthStencilState, const std::shared_ptr<ciri::IRasterizerState>& rasterizerState, SpriteSortMode sortMode, const std::shared_ptr<ciri::IShader>& shader ) {
	// check for bad inputs (shader is optional and will be set to a default one if it is null, so don't check it)
	if( nullptr == blendState || nullptr == samplerState || nullptr == depthStencilState || nullptr == rasterizerState ) {
		return false;
	}

	// save custom shader if provided
	_shader = (shader != nullptr) ? shader : _defaultShader;
	if( shader != nullptr ) {
	} else {
	}

	// must have a valid shader set
	if( !_shader->isValid() ) {
		return false;
	}

	_blendState = blendState;
	_samplerState = samplerState;
	_depthStencilState = depthStencilState;
	_rasterizerState = rasterizerState;
	_sortMode = sortMode;

	_beginCalled = true;

	return true;
}

void SpriteBatch::draw( const std::shared_ptr<ciri::ITexture2D>& texture, const cc::Vec4f& dstRect, float rotation, const cc::Vec2f& origin, float depth ) {
	if( nullptr == texture ) {
		return;
	}

	std::shared_ptr<SpriteBatchItem> item = createBatchItem();
	item->texture = texture;

	const float textureWidth = static_cast<float>(texture->getWidth());
	const float textureHeight = static_cast<float>(texture->getHeight());
	const cc::Vec2f newOrigin(origin.x * (dstRect.z / textureWidth), origin.y * (dstRect.w / textureHeight));
	item->set(dstRect.x, dstRect.y, -newOrigin.x, -newOrigin.y, dstRect.z, dstRect.w, sinf(rotation), cosf(rotation), depth, cc::Vec4f(1.0f));
}

void SpriteBatch::draw( const std::shared_ptr<ciri::ITexture2D>& texture, const cc::Vec2f& position, float rotation, const cc::Vec2f& origin, const cc::Vec2f& scale, float depth ) {
	if( nullptr == texture ) {
		return;
	}

	std::shared_ptr<SpriteBatchItem> item = createBatchItem();
	item->texture = texture;

	const float textureWidth = static_cast<float>(texture->getWidth()) * scale.x;
	const float textureHeight = static_cast<float>(texture->getHeight()) * scale.y;
	const cc::Vec2f newOrigin = origin * scale;
	item->set(position.x, position.y, -newOrigin.x, -newOrigin.y, textureWidth, textureHeight, sinf(rotation), cosf(rotation), depth, cc::Vec4f(1.0f));
}

void SpriteBatch::draw( const std::shared_ptr<ciri::ITexture2D>& texture, const cc::Vec2f& position, float rotation, const cc::Vec2f& origin, float scale, float depth ) {
	if( nullptr == texture ) {
		return;
	}

	std::shared_ptr<SpriteBatchItem> item = createBatchItem();
	item->texture = texture;

	const float textureWidth = static_cast<float>(texture->getWidth()) * scale;
	const float textureHeight = static_cast<float>(texture->getHeight()) * scale;
	const cc::Vec2f newOrigin = origin * scale;
	item->set(position.x, position.y, -newOrigin.x, -newOrigin.y, textureWidth, textureHeight, sinf(rotation), cosf(rotation), depth, cc::Vec4f(1.0f));
}

void SpriteBatch::draw( const std::shared_ptr<ciri::ITexture2D>& texture, const cc::Vec2f& position, float rotation, const cc::Vec2f& origin, const cc::Vec2f& scale, float depth, const cc::Vec4f& color ) {
	if( nullptr == texture ) {
		return;
	}

	std::shared_ptr<SpriteBatchItem> item = createBatchItem();
	item->texture = texture;

	const float textureWidth = static_cast<float>(texture->getWidth()) * scale.x;
	const float textureHeight = static_cast<float>(texture->getHeight()) * scale.y;
	const cc::Vec2f newOrigin = origin * scale;
	item->set(position.x, position.y, -newOrigin.x, -newOrigin.y, textureWidth, textureHeight, sinf(rotation), cosf(rotation), depth, color);
}

//void SpriteBatch::drawString( const std::string& text, const std::shared_ptr<ISpriteFont>& font, const cc::Vec2f& position, const cc::Vec4f& color ) {
void SpriteBatch::drawString( const std::shared_ptr<ISpriteFont>& font, const std::string& text, const cc::Vec2f& position, const cc::Vec4f& color, float scale, float rotation, float depth ) {
	if( nullptr == font || text.empty() ) {
		return;
	}

	const cc::Vec2f SCALE(scale, scale);
	cc::Vec2i offset(0, 0);

	for( const auto& c : text ) {
		// ignore carriage return
		if( '\r' == c ) {
			continue;
		}

		// handle newline
		if( '\n' == c ) {
			offset.x = 0;
			offset.y -= font->getLineSpacing();
			continue;
		}

		SpriteFontGlyph glyph;
		if( !font->getGlyph(c, glyph) ) {
			printf("SpriteBatch::drawString Failed to get glyph.\n");
			continue;
		}

		// if no texture, just advance (could be a space, for instance)
		if( nullptr == glyph.texture() ) {
			offset.x += (glyph.advance() >> 6) * scale;
			continue;
		}

		const float xPos = offset.x + position.x + glyph.bearingLeft() * scale;
		const float yPos = offset.y + position.y - (glyph.height() - glyph.bearingTop()) * scale;

		const float width = glyph.width() * scale;
		const float height = glyph.height() * scale;

		const cc::Vec2f origin(0.0f, 0.0f);
		draw(glyph.texture(), cc::Vec2f(xPos, yPos), rotation, origin, SCALE, depth, color);

		offset.x += (glyph.advance() >> 6) * scale;
	}
}

bool SpriteBatch::end() {
	// cannot end without begin
	if( false == _beginCalled ) {
		return false;
	}

	// no longer beginning
	_beginCalled = false;

	// check for no items
	if( 0 == _batchItemList.size() ) {
		return true; // not an error to have no batches
	}

	const int batchCount = _batchItemList.size();

	ensureArrayCapacity(batchCount);

	// sort array
	switch( _sortMode ) {
		case SpriteSortMode::Texture: {
			std::sort(_batchItemList.begin(), _batchItemList.end(), SpriteSortTexture());
			break;
		}

		case SpriteSortMode::FrontToBack: {
			std::sort(_batchItemList.begin(), _batchItemList.end(), SpriteSortFrontToBack());
			break;
		}

		case SpriteSortMode::BackToFront: {
			std::sort(_batchItemList.begin(), _batchItemList.end(), SpriteSortBackToFront());
			break;
		}

		default: {
			break;
		}
	}
	
	// update vertex array
	int batchIndex = 0;
	for( int i = 0; i < batchCount; ++i ) {
		std::shared_ptr<SpriteBatchItem> item = _batchItemList[i];
		_vertexArray[batchIndex++] = item->topLeft;     // t0
		_vertexArray[batchIndex++] = item->bottomRight; // t0
		_vertexArray[batchIndex++] = item->bottomLeft;  // t0
		_vertexArray[batchIndex++] = item->topLeft;     // t1
		_vertexArray[batchIndex++] = item->topRight;    // t1
		_vertexArray[batchIndex++] = item->bottomRight; // t1
	}

	// configure gpu resources
	configure();

	// draw batched
	int startIndex = 0;
	int endIndex = 0;
	std::shared_ptr<ciri::ITexture2D> boundTexture = nullptr;
	for( int i = 0; i < batchCount; ++i ) {
		std::shared_ptr<SpriteBatchItem> item = _batchItemList[i];
		// if texture has changed...
		if( item->texture != boundTexture ) {
			// draw what is already backlogged
			flush(startIndex, endIndex, boundTexture);

			// bind new texture
			boundTexture = item->texture;

			// reset start index
			startIndex = i * 6;
		}

		// update end index
		endIndex += 1 * 6;

		item->texture = nullptr;
		_freeBatchItemQueue.push(item);
	}
	// draw remaining items
	flush(startIndex, endIndex, boundTexture);

	// discard batched items
	_batchItemList.clear();

	// reset gpu states
	_device->setVertexBuffer(nullptr);
	_device->setBlendState(nullptr);
	_device->setDepthStencilState(nullptr);
	_device->setRasterizerState(nullptr);
	_device->setSamplerState(0, nullptr, ciri::ShaderStage::Pixel);
	_device->setTexture2D(0, nullptr, ciri::ShaderStage::Pixel);
	_device->applyShader(nullptr);

	return true;
}

void SpriteBatch::clean() {
	_beginCalled = false;

	_spritesBuffer = nullptr;

	_constantBuffer = nullptr;

	_defaultShader = nullptr;

	_shader = nullptr;
}

bool SpriteBatch::configure() {
	_device->setBlendState(_blendState);
	_device->setDepthStencilState(_depthStencilState);
	_device->setRasterizerState(_rasterizerState);
	_device->setSamplerState(0, _samplerState, ciri::ShaderStage::Pixel);

	const ciri::Viewport& vp = _device->getViewport();

	// update constant buffer
	_constants.projection = cc::math::orthographic(0.0f, static_cast<float>(vp.width()), 0.0f, static_cast<float>(vp.height()), -1.0f, 1.0f);
	if( ciri::failed(_constantBuffer->setData(sizeof(SpriteConstants), &_constants)) ) {
		return false;
	}

	// update vertex buffer
	if( ciri::failed(_spritesBuffer->set(_vertexArray, sizeof(SpriteVertex), _vertexArraySize, true)) ) {
		return false;
	}

	// set shader
	_device->applyShader(_shader);

	// set vertex buffer
	_device->setVertexBuffer(_spritesBuffer);

	return true;
}

std::shared_ptr<SpriteBatchItem> SpriteBatch::createBatchItem() {
	std::shared_ptr<SpriteBatchItem> item = nullptr;
	if( _freeBatchItemQueue.size() > 0 ) {
		item = _freeBatchItemQueue.front();
		_freeBatchItemQueue.pop();
	} else {
		item = std::make_shared<SpriteBatchItem>();
	}
	_batchItemList.push_back(item);
	return item;
}

void SpriteBatch::ensureArrayCapacity( int size ) {
	const int requiredSize = size * 6;
	if( _vertexArraySize < requiredSize ) {
		SpriteVertex* newArray = new SpriteVertex[requiredSize];
		memcpy(newArray, _vertexArray, sizeof(SpriteVertex) * _vertexArraySize);
		delete[] _vertexArray;
		_vertexArray = newArray;
		_vertexArraySize = requiredSize;
	}
}

void SpriteBatch::flush( int start, int end, const std::shared_ptr<ciri::ITexture2D>& texture ) {
	if( start == end ) {
		return;
	}

	_device->setTexture2D(0, texture, ciri::ShaderStage::Pixel);

	const int count = end - start;
	_device->drawArrays(ciri::PrimitiveTopology::TriangleList, count, start);
}