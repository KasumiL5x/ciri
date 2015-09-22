#include "SpriteBatch.hpp"
#include <cc/MatrixFunc.hpp>

SpriteBatch::SpriteBatch()
	: _beginCalled(false), MAX_SPRITES(10), _sprites(nullptr) {
}

SpriteBatch::~SpriteBatch() {
}

bool SpriteBatch::create( const std::shared_ptr<ciri::IGraphicsDevice>& device ) {
	// store gfx device
	_device = device;

	// create sprites data array
	_sprites = new SpriteVertex[MAX_SPRITES];
	// create and set sprites data vertex buffer
	_spritesBuffer = device->createVertexBuffer();
	if( ciri::failed(_spritesBuffer->set(_sprites, sizeof(SpriteVertex), MAX_SPRITES, true)) ) {
		return false;
	}

	// load and configure shader and constants
	_shader = device->createShader();
	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float2, ciri::VertexUsage::Position, 0));
	_shader->addInputElement(ciri::VertexElement(ciri::VertexFormat::Float2, ciri::VertexUsage::Texcoord, 0));
	const std::string shaderExt = device->getShaderExt();
	const std::string vsFile = ("sprites/shaders/SpriteBatch_vs" + shaderExt);
	const std::string gsFile = ("sprites/shaders/SpriteBatch_gs" + shaderExt);
	const std::string psFile = ("sprites/shaders/SpriteBatch_ps" + shaderExt);
	if( ciri::failed(_shader->loadFromFile(vsFile.c_str(), gsFile.c_str(), psFile.c_str())) ) {
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
	if( ciri::failed(_shader->addConstants(_constantBuffer, "SpriteConstants", ciri::ShaderStage::Geometry)) ) {
		printf("Failed to assign constants to SpriteBatch shader.\n");
		return false;
	}

	return true;
}

bool SpriteBatch::begin( const std::shared_ptr<ciri::IBlendState>& blendState, const std::shared_ptr<ciri::ISamplerState>& samplerState, const std::shared_ptr<ciri::IDepthStencilState>& depthStencilState, const std::shared_ptr<ciri::IRasterizerState>& rasterizerState ) {
	// check for bad inputs
	if( nullptr == blendState || nullptr == samplerState || nullptr == depthStencilState || nullptr == rasterizerState || nullptr == _shader ) {
		return false;
	}

	_blendState = blendState;
	_samplerState = samplerState;
	_depthStencilState = depthStencilState;
	_rasterizerState = rasterizerState;

	_beginCalled = true;

	return true;
}

bool SpriteBatch::draw( const std::shared_ptr<ciri::ITexture2D>& texture, const cc::Vec2f& position, const cc::Vec2f& scale ) {
	if( false == _beginCalled ) {
		return false;
	}

	// get next valid index in buffer
	const int idx = getNextFreeIndex();
	if( -1 == idx ) {
		return false;
	}

	// set sprite's data in the vertex buffer
	_sprites[idx].position = position;
	_sprites[idx].scale = scale;

	// batch an item that references the idx with the texture
	SpriteBatchItem item;
	item.vbIndex = idx;
	item.texture = texture;
	_batchedItems.push(item);

	return true;
}

bool SpriteBatch::end() {
	// cannot end without begin
	if( false == _beginCalled ) {
		return false;
	}

	// no longer beginning
	_beginCalled = false;

	// check for no items
	if( 0 == _batchedItems.size() ) {
		return true; // not an error to have no batches
	}

	// configure gpu resources
	configure();

	while( !_batchedItems.empty() ) {
		const SpriteBatchItem& item = _batchedItems.front();

		// set the item's texture
		_device->setTexture2D(0, item.texture.lock(), ciri::ShaderStage::Pixel);

		// draw one element
		_device->drawArrays(ciri::PrimitiveTopology::PointList, 1, item.vbIndex);

		// popppppppp
		_batchedItems.pop();
	}

	// reset gpu states
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

	if( _sprites != nullptr ) {
		delete[] _sprites;
		_sprites = nullptr;
	}

	_spritesBuffer = nullptr;

	_constantBuffer = nullptr;

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
	_constants.screenSize.x = vp.width();
	_constants.screenSize.y = vp.height();
	if( ciri::failed(_constantBuffer->setData(sizeof(SpriteConstants), &_constants)) ) {
		return false;
	}

	// todo: only update when necessary?
	// update vertex buffer
	if( ciri::failed(_spritesBuffer->set(_sprites, sizeof(SpriteVertex), MAX_SPRITES, true)) ) {
		return false;
	}

	// set shader
	_device->applyShader(_shader);

	// set vertex buffer
	_device->setVertexBuffer(_spritesBuffer);
}

int SpriteBatch::getNextFreeIndex() const {
	return _batchedItems.size();
}