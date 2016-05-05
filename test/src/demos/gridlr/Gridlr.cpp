#include "Gridlr.hpp"
#include <cctype>

Gridlr::Gridlr()
	: Game(), _grid(nullptr), _blendState(nullptr), _samplerState(nullptr), _depthStencilState(nullptr), _rasterizerState(nullptr), _cellTexture(nullptr),
		_isDragging(false), _startDragBlock(0, 0), _lastDragState(CellState::Empty) {
	_config.width = 1280;
	_config.height = 720;
	_config.title = "ciri : Gridlr";
}

Gridlr::~Gridlr() {
}

void Gridlr::onInitialize() {
	Game::onInitialize();

	// make new empty grid
	_grid = std::make_unique<GridlrGrid>();
	_grid->create(8,8);

	//
	// test intersecting path truncation
	_grid->getCell(0, 0)->setState(CellState::Red);
	_grid->getCell(0, 0)->setIsSource(true);
	_grid->getCell(4, 4)->setState(CellState::Red);
	_grid->getCell(4, 4)->setIsEnd(true);
	//// intersecting path
	//_grid->getCell(1, 0)->setState(CellState::Red);
	//_grid->getCell(1, 1)->setState(CellState::Red); // this one
	//_grid->getCell(1, 2)->setState(CellState::Red);
	//_grid->getCell(1, 3)->setState(CellState::Red);
	//_grid->getCell(1, 4)->setState(CellState::Red);
	//_grid->getCell(2, 4)->setState(CellState::Red);
	//_grid->getCell(3, 4)->setState(CellState::Red);
	//_grid->getCell(3, 3)->setState(CellState::Red);
	//_grid->getCell(4, 3)->setState(CellState::Red);
	//_grid->getCell(4, 2)->setState(CellState::Red);
	//_grid->getCell(4, 1)->setState(CellState::Red);
	//_grid->getCell(3, 1)->setState(CellState::Red);
	//_grid->getCell(2, 1)->setState(CellState::Red); // this one
	//// truncate isn't working like the python version... :(
	//printf("%s\n", _grid->truncate(*_grid->getCell(1, 1), true) ? "Truncated successfully" : "Failed to truncate");
	_grid->print();

	// print driver info
	printf("Device: %s\n", graphicsDevice()->getGpuName());
	printf("API: %s\n", graphicsDevice()->getApiInfo());

	// create spritebatch
	printf(_spriteBatch.create(graphicsDevice()) ? "Initialized SpriteBatch.\n" : "Failed to initialize SpriteBatch.\n");

	// create states
	_blendState = graphicsDevice()->getDefaultBlendAlpha();
	_samplerState = graphicsDevice()->createSamplerState(ciri::SamplerDesc());
	_depthStencilState = graphicsDevice()->getDefaultDepthStencilNone();
	_rasterizerState = graphicsDevice()->getDefaultRasterCounterClockwise();
}

void Gridlr::onLoadContent() {
	Game::onLoadContent();

	// load grid texture
	ciri::PNG cellPng;
	if( cellPng.loadFromFile("gridlr/grid_empty.png", true) && cellPng.hasAlpha() ) {
		_cellTexture = graphicsDevice()->createTexture2D(cellPng.getWidth(), cellPng.getHeight(), ciri::TextureFormat::RGBA32_UINT, 0, cellPng.getPixels());
		const auto& vp = graphicsDevice()->getViewport();
		_gridOffset.x = static_cast<float>((vp.width()  / 2) - (_cellTexture->getWidth()  * _grid->getWidth()  / 2));
		_gridOffset.y = static_cast<float>((vp.height() / 2) - (_cellTexture->getHeight() * _grid->getHeight() / 2));
	}
}

void Gridlr::onEvent(const ciri::WindowEvent& evt) {
	Game::onEvent(evt);
}

void Gridlr::onUpdate(const double deltaTime, const double elapsedTime) {
	Game::onUpdate(deltaTime, elapsedTime);

	// check for close w/ escape
	if( window()->hasFocus() && input()->isKeyDown(ciri::Key::Escape) ) {
		gtfo();
		return;
	}

	// compute which cell the mouse is under
	const int mouseCellX = (input()->mouseX() - static_cast<int>(_gridOffset.x)) / _cellTexture->getWidth();
	const int mouseCellY = (input()->mouseY() - static_cast<int>(_gridOffset.y)) / _cellTexture->getHeight();
	//printf("X: %d; Y: %d\n", mouseCellX, mouseCellY);
	//printf("%d\n", input()->mouseY());

}

void Gridlr::onFixedUpdate(const double deltaTime, const double elapsedTime) {
	Game::onFixedUpdate(deltaTime, elapsedTime);
}

void Gridlr::onDraw() {
	Game::onDraw();

	const auto device = graphicsDevice();

	// clear screen
	device->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);

	_spriteBatch.begin(_blendState, _samplerState, _depthStencilState, _rasterizerState, SpriteSortMode::Deferred, nullptr);
	// draw grid
	for( int y = 0; y < _grid->getHeight(); ++y ) {
		for( int x = 0; x < _grid->getWidth(); ++x ) {
			auto cell = _grid->getCell(x, y);

			// vp to invert positions
			auto& vp = device->getViewport();

			const cc::Vec2f position = _gridOffset + cc::Vec2f(static_cast<float>(_cellTexture->getWidth() * x), static_cast<float>(_cellTexture->getHeight() * y));
			const cc::Vec2f origin(0.0f, 0.0f);
			const cc::Vec2f scale(1.0f, 1.0f);
			cc::Vec4f color;
			switch( cell->state() ) {
				case CellState::Empty: {
					color = cc::Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
					break;
				}
				case CellState::Red: {
					color = cc::Vec4f(1.0f, 0.0f, 0.0f, 1.0f);
					break;
				}
				case CellState::Green: {
					color = cc::Vec4f(0.0f, 1.0f, 0.0f, 1.0f);
					break;
				}
				case CellState::Blue: {
					color = cc::Vec4f(0.0f, 0.0f, 1.0f, 1.0f);
					break;
				}
			}
			_spriteBatch.draw(_cellTexture, position, 0.0f, origin, scale, 0.0f, color);
		}
	}
	_spriteBatch.end();

	// present to screen
	device->present();
}

void Gridlr::onUnloadContent() {
	Game::onUnloadContent();

	// clean spritebatch
	_spriteBatch.clean();
}