#include "SpriteBatch.hpp"
#include <cc/MatrixFunc.hpp>

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
	: _beginCalled(false), _sortMode(SpriteSortMode::Deferred) {
}

SpriteBatch::~SpriteBatch() {
}

bool SpriteBatch::create( const std::shared_ptr<ciri::IGraphicsDevice>& device ) {
	// store gfx device
	_device = device;

	_spritesBuffer = device->createVertexBuffer();

	//ensureArrayCapacity(10);

	// load and configure shader and constants
	_shader = device->createShader();
	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Position, 0));
	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float2, ciri::VertexUsage::Texcoord, 0));
	const std::string shaderExt = device->getShaderExt();
	const std::string vsFile = ("sprites/shaders/SpriteBatch_vs" + shaderExt);
	//const std::string gsFile = ("sprites/shaders/SpriteBatch_gs" + shaderExt);
	const std::string psFile = ("sprites/shaders/SpriteBatch_ps" + shaderExt);
	if( ciri::failed(_shader->loadFromFile(vsFile.c_str(), nullptr, psFile.c_str())) ) {
		printf("Failed to load SpriteBatch shader:\n");
		for( auto err : _shader->getErrors() ) {
			printf("%s\n", err.msg.c_str());
		}
		return false;
	}
	_constantBuffer = device->createConstantBuffer();
	if( ciri::failed(_constantBuffer->setData(sizeof(SpriteConstants), &_constants)) ) {
		printf("Failed to create SpriteBatch constants.\n");
		return false;
	}
	if( ciri::failed(_shader->addConstants(_constantBuffer, "SpriteConstants", ciri::ShaderStage::Vertex)) ) {
		printf("Failed to assign constants to SpriteBatch shader.\n");
		return false;
	}

	return true;
}

bool SpriteBatch::begin( const std::shared_ptr<ciri::IBlendState>& blendState, const std::shared_ptr<ciri::ISamplerState>& samplerState, const std::shared_ptr<ciri::IDepthStencilState>& depthStencilState, const std::shared_ptr<ciri::IRasterizerState>& rasterizerState, SpriteSortMode sortMode ) {
	// check for bad inputs
	if( nullptr == blendState || nullptr == samplerState || nullptr == depthStencilState || nullptr == rasterizerState || nullptr == _shader ) {
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
	std::shared_ptr<SpriteBatchItem> item = createBatchItem();
	item->texture = texture;

	const float textureWidth = static_cast<float>(texture->getWidth());
	const float textureHeight = static_cast<float>(texture->getHeight());
	const cc::Vec2f newOrigin(origin.x * (dstRect.z / textureWidth), origin.y * (dstRect.w / textureHeight));
	item->set(dstRect.x, dstRect.y, -newOrigin.x, -newOrigin.y, dstRect.z, dstRect.w, sinf(rotation), cosf(rotation), depth);
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
			startIndex = i;
		}

		// update end index
		endIndex = i+1;

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

	_shader = nullptr;
}

void SpriteBatch::debugReloadShaders() {
	if( _beginCalled ) {
		return;
	}

	_shader->destroy();

	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float3, ciri::VertexUsage::Position, 0));
	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float2, ciri::VertexUsage::Texcoord, 0));
	const std::string shaderExt = _device->getShaderExt();
	const std::string vsFile = ("sprites/shaders/SpriteBatch_vs" + shaderExt);
	//const std::string gsFile = ("sprites/shaders/SpriteBatch_gs" + shaderExt);
	const std::string psFile = ("sprites/shaders/SpriteBatch_ps" + shaderExt);
	if( ciri::failed(_shader->loadFromFile(vsFile.c_str(), nullptr, psFile.c_str())) ) {
		printf("Failed to load SpriteBatch shader:\n");
		for( auto err : _shader->getErrors() ) {
			printf("%s\n", err.msg.c_str());
		}
		return;
	}
	if( ciri::failed(_constantBuffer->setData(sizeof(SpriteConstants), &_constants)) ) {
		printf("Failed to create SpriteBatch constants.\n");
		return ;
	}
	if( ciri::failed(_shader->addConstants(_constantBuffer, "SpriteConstants", ciri::ShaderStage::Vertex)) ) {
		printf("Failed to assign constants to SpriteBatch shader.\n");
		return;
	}
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
	if( ciri::failed(_spritesBuffer->set(_vertexArray.data(), sizeof(SpriteVertex), _vertexArray.size(), true)) ) {
		return false;
	}

	// set shader
	_device->applyShader(_shader);

	// set vertex buffer
	_device->setVertexBuffer(_spritesBuffer);
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
	if( _vertexArray.size() < requiredSize ) {
		_vertexArray.resize(requiredSize);
	}
}

void SpriteBatch::flush( int start, int end, const std::shared_ptr<ciri::ITexture2D>& texture ) {
	if( start == end ) {
		return;
	}

	_device->setTexture2D(0, texture, ciri::ShaderStage::Pixel);

	const int count = end - start;
	_device->drawArrays(ciri::PrimitiveTopology::TriangleList, count * 6, start);
}