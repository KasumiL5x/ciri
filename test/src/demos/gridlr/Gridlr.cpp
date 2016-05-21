#include "Gridlr.hpp"
#include <cctype>
#include <array>
#include <cc/Random.hpp>

Gridlr::Gridlr()
	: App(), _grid(nullptr), _blendState(nullptr), _samplerState(nullptr), _depthStencilState(nullptr), _rasterizerState(nullptr), _cellTexture(nullptr),
		_isDragging(false) {
	_config.width = 1280;
	_config.height = 720;
	_config.title = "ciri : Gridlr";
}

Gridlr::~Gridlr() {
}

void Gridlr::onInitialize() {
	App::onInitialize();

	// cell off grid to start
	_lastMouseCell.x = -1;
	_lastMouseCell.y = -1;

	// make new empty grid
	_grid = std::make_unique<gridlr::BlockGrid>();
	_grid->create(8, 8);

	// enable chains
	_grid->enableChain(gridlr::BlockState::One, *_grid->get(0, 0), *_grid->get(4, 4));
	_grid->enableChain(gridlr::BlockState::Two, *_grid->get(4, 6), *_grid->get(6, 2));
	_grid->enableChain(gridlr::BlockState::Three, *_grid->get(1, 4), *_grid->get(5, 6));

	// debug print grid
	_grid->print();

	// print driver info
	printf("Device: %s\n", graphicsDevice()->getGpuName());
	printf("API: %s\n", graphicsDevice()->getApiInfo());

	// create spritebatch
	printf(_spriteBatch.create(graphicsDevice()) ? "Initialized SpriteBatch.\n" : "Failed to initialize SpriteBatch.\n");

	// create states
	_blendState = graphicsDevice()->getDefaultBlendAlpha();
	ciri::SamplerDesc samplerDesc;
	samplerDesc.filter = ciri::SamplerFilter::Point;
	samplerDesc.wrapU=samplerDesc.wrapV=samplerDesc.wrapW = ciri::SamplerWrap::Clamp;
	_samplerState = graphicsDevice()->createSamplerState(samplerDesc);
	_depthStencilState = graphicsDevice()->getDefaultDepthStencilNone();
	_rasterizerState = graphicsDevice()->getDefaultRasterCounterClockwise();
}

void Gridlr::onLoadContent() {
	App::onLoadContent();

	// load grid texture
	ciri::PNG cellPng;
	if( cellPng.loadFromFile("gridlr/grid_empty.png", true) && cellPng.hasAlpha() ) {
		_cellTexture = graphicsDevice()->createTexture2D(cellPng.getWidth(), cellPng.getHeight(), ciri::TextureFormat::RGBA32_UINT, 0, cellPng.getPixels());
		const auto& vp = graphicsDevice()->getViewport();
		_gridOffset.x = static_cast<float>((vp.width()  / 2) - (_cellTexture->getWidth()  * _grid->width()  / 2));
		_gridOffset.y = static_cast<float>((vp.height() / 2) - (_cellTexture->getHeight() * _grid->height() / 2));
	}

	// load sprite font
	_font = std::make_shared<ciri::FreeTypeSpriteFont>(graphicsDevice());
	if( ciri::failed(_font->loadFromFile("data/fonts/Gravity-Bold.ttf")) ) {
		printf("Failed to load font.\n");
	}
	_font->setSize(72);
	_font->setLineSpacing(72);
}

void Gridlr::onEvent(const ciri::WindowEvent& evt) {
	App::onEvent(evt);
}

void Gridlr::onUpdate(const double deltaTime, const double elapsedTime) {
	App::onUpdate(deltaTime, elapsedTime);

	// check for close w/ escape
	if( window()->hasFocus() && input()->isKeyDown(ciri::Key::Escape) ) {
		gtfo();
		return;
	}

	// compute which cell the mouse is under
	const int mouseCellX = (input()->mouseX() - static_cast<int>(_gridOffset.x)) / _cellTexture->getWidth();
	const int mouseCellY = (input()->mouseY() - static_cast<int>(_gridOffset.y)) / _cellTexture->getHeight();
	//printf("X: %d; Y: %d\n", mouseCellX, mouseCellY);

	if( input()->isMouseButtonDown(ciri::MouseButton::Left) && !_isDragging ) {
		_isDragging = true;
		_lastMouseCell.x = mouseCellX;
		_lastMouseCell.y = mouseCellY;
		// start drag
		auto mouseBlock = _grid->get(mouseCellX, mouseCellY);
		if( mouseBlock != nullptr ) {
			if( !_grid->startDrag(*mouseBlock) ) {
				printf("BlockGrid::startDrag() failed.\n");
			}
		}
	} else if( input()->isMouseButtonDown(ciri::MouseButton::Left) && _isDragging && (_lastMouseCell.x != mouseCellX || _lastMouseCell.y != mouseCellY) ) {
		_lastMouseCell.x = mouseCellX;
		_lastMouseCell.y = mouseCellY;

		// on drag
		auto mouseBlock = _grid->get(mouseCellX, mouseCellY);
		if( mouseBlock != nullptr ) {
			if( !_grid->dragMove(*mouseBlock) ) {
				printf("BlockGrid::dragMove() failed.\n");
			}
		}
	} else if( input()->isMouseButtonUp(ciri::MouseButton::Left) && _isDragging ) {
		_isDragging = false;

		// stop drag
		auto mouseBlock = _grid->get(mouseCellX, mouseCellY);
		if( mouseBlock != nullptr ) {
			if( !_grid->stopDrag(*mouseBlock) ) {
				printf("BlockGrid::stopDrag() failed.\n");
			}
		}
	}

	if( _grid->isComplete() ) {
		printf("Complete!\n");
	}




	//const auto size = _font->measureString("hello");
	//printf("Size: %dpx x %dpx\n", size.x, size.y);
}

void Gridlr::onFixedUpdate(const double deltaTime, const double elapsedTime) {
	App::onFixedUpdate(deltaTime, elapsedTime);
}

void Gridlr::onDraw() {
	App::onDraw();

	const auto device = graphicsDevice();

	// clear screen
	device->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);

	_spriteBatch.begin(_blendState, _samplerState, _depthStencilState, _rasterizerState, ciri::SpriteSortMode::Deferred, nullptr);
	// draw grid
	for( int y = 0; y < _grid->height(); ++y ) {
		for( int x = 0; x < _grid->width(); ++x ) {
			auto block = _grid->get(x, y);

			// vp to invert positions
			auto& vp = device->getViewport();

			// offset and inverted positions
			cc::Vec2f position = {
				_gridOffset.x + static_cast<float>(_cellTexture->getWidth() * x),
				_gridOffset.y + static_cast<float>(_cellTexture->getHeight() * y)
			};

			// pick color based on state
			cc::Vec4f color;
			switch( block->state() ) {
				case gridlr::BlockState::Empty: { color = cc::Vec4f(1.0f, 1.0f, 1.0f, 1.0); break; }
				case gridlr::BlockState::One: { color = cc::Vec4f(1.0f, 0.0f, 0.0f, 1.0); break; }
				case gridlr::BlockState::Two: { color = cc::Vec4f(0.0f, 1.0f, 0.0f, 1.0); break; }
				case gridlr::BlockState::Three: { color = cc::Vec4f(0.0f, 0.0f, 1.0f, 1.0); break; }
				default: { color = cc::Vec4f(1.0f, 0.0f, 1.0f, 1.0f); break; }
			}

			// draw
			const cc::Vec2f origin(0.0f, 0.0f);
			const cc::Vec2f scale(1.0f, 1.0f);
			_spriteBatch.draw(_cellTexture, position, 0.0f, origin, scale, 0.0f, color);
		}
	}

	//const std::string str1 = "Hello, ";
	//const cc::Vec2f str1pos(input()->mouseX(), input()->mouseY());
	//_spriteBatch.drawString(_font, str1, str1pos, cc::Vec4f(0.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0.0f, 1.0f);
	//const std::string str2 = "World!";
	//const cc::Vec2f str2pos = str1pos + cc::Vec2f(_font->measureString(str1).x, 0.0f);
	//_spriteBatch.drawString(_font, str2, str2pos, cc::Vec4f(1.0f, 0.0f, 1.0f, 1.0f), 1.0f, 0.0f, 1.0f);
	//_spriteBatch.drawString(_font, "Hello, World!", str1pos + cc::Vec2f(0.0f, 100.0f), cc::Vec4f(1.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0.0f, 1.0f);
	//printf("x: %d; y: %d\n", input()->mouseX(), input()->mouseY());

	_spriteBatch.end();

	// present to screen
	device->present();
}

void Gridlr::onUnloadContent() {
	App::onUnloadContent();

	// clean spritebatch
	_spriteBatch.clean();
}