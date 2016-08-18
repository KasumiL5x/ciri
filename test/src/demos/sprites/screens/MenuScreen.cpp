#include "MenuScreen.hpp"

MenuScreen::MenuScreen( const std::shared_ptr<ciri::IGraphicsDevice>& device, const std::shared_ptr<ciri::SpriteBatch>& spriteBatch, const std::shared_ptr<ciri::IInput>& input )
	: Screen(), _device(device), _spriteBatch(spriteBatch), _input(input) {
}

MenuScreen::~MenuScreen() {
}

void MenuScreen::onAdd() {
	_selectedMenuIndex = 0;

	// load sprite font
	_titleFont = std::make_shared<ciri::FreeTypeSpriteFont>(_device);
	if( ciri::failed(_titleFont->loadFromFile("data/fonts/Gravity-Light.ttf")) ) {
		printf("Failed to load font.\n");
	}
	_titleFont->setSize(100);
	_menuFont = std::make_shared<ciri::FreeTypeSpriteFont>(_device);
	if( ciri::failed(_menuFont->loadFromFile("data/fonts/Gravity-Light.ttf")) ) {
		printf("Failed to load menu font.\n");
	}
	_menuFont->setSize(42);

	// load states
	_blendState = _device->getDefaultBlendAlpha();
	auto sampleDesc = ciri::SamplerDesc();
	sampleDesc.filter = ciri::SamplerFilter::Point;
	_samplerState = _device->createSamplerState(sampleDesc);
	_depthStencilState = _device->getDefaultDepthStencilNone();
	_rasterizerState = _device->getDefaultRasterCounterClockwise();
}

void MenuScreen::onRemove() {
}

void MenuScreen::onUpdate( double deltaTime, double elapsedTime ) {
	if( _input->isKeyDown(ciri::Key::Up) && _input->wasKeyUp(ciri::Key::Up) ) {
		_selectedMenuIndex -= 1;
		_selectedMenuIndex = _selectedMenuIndex < 0 ? 0 : _selectedMenuIndex;
	}
	if( _input->isKeyDown(ciri::Key::Down) && _input->wasKeyUp(ciri::Key::Down) ) {
		_selectedMenuIndex += 1;
		_selectedMenuIndex = _selectedMenuIndex > 3 ? 3 : _selectedMenuIndex;
	}
}

void MenuScreen::onFixedUpdate( double deltaTime, double elapsedTime ) {
}

void MenuScreen::onDraw() {

	_device->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	_device->clear(ciri::ClearFlags::Color | ciri::ClearFlags::Depth);

	const auto& vp = _device->getViewport();

	_spriteBatch->begin(_blendState, _samplerState, _depthStencilState, _rasterizerState, ciri::SpriteSortMode::Deferred, nullptr);
	
	const cc::Vec4f SEL_CLR = cc::Vec4f(0.7f, 0.4f, 0.8f, 1.0f);
	const cc::Vec4f UNSEL_CLR = cc::Vec4f(1.0f);

	// title
	const std::string title = "Geometry Wars";
	const cc::Vec2i titleSize = _titleFont->measureString(title);
	const cc::Vec2f titlePos = cc::Vec2f(vp.width()/2-titleSize.x/2, vp.height()-titleSize.y*2);
	_spriteBatch->drawString(_titleFont, title, titlePos, cc::Vec4f(0.85f, 0.85f, 0.85f, 1.0f), 1.0f, 0.0f, 0.0f);

	// new game
	const std::string newGame = "New Game";
	const cc::Vec2i newGameSize = _menuFont->measureString(newGame);
	const cc::Vec2f newGamePos = cc::Vec2f(vp.width()/2-newGameSize.x/2, titlePos.y-newGameSize.y*5);
	_spriteBatch->drawString(_menuFont, newGame, newGamePos, _selectedMenuIndex==0?SEL_CLR:UNSEL_CLR, 1.0f, 0.0f, 0.0f);

	// leaderboard
	const std::string board = "Leaderboard";
	const cc::Vec2i boardSize = _menuFont->measureString(board);
	const cc::Vec2f boardPos = cc::Vec2f(vp.width()/2-boardSize.x/2, newGamePos.y - boardSize.y*2);
	_spriteBatch->drawString(_menuFont, board, boardPos, _selectedMenuIndex==1?SEL_CLR:UNSEL_CLR, 1.0f, 0.0f, 0.0f);

	// credits
	const std::string credits = "Credits";
	const cc::Vec2i creditsSize = _menuFont->measureString(credits);
	const cc::Vec2f creditsPos = cc::Vec2f(vp.width()/2-creditsSize.x/2, boardPos.y-creditsSize.y*2);
	_spriteBatch->drawString(_menuFont, credits, creditsPos, _selectedMenuIndex==2?SEL_CLR:UNSEL_CLR, 1.0f, 0.0f, 0.0f);

	// exit
	const std::string exit = "Exit";
	const cc::Vec2i exitSize = _menuFont->measureString(exit);
	const cc::Vec2f exitPos = cc::Vec2f(vp.width()/2-exitSize.x/2, creditsPos.y-exitSize.y*2);
	_spriteBatch->drawString(_menuFont, exit, exitPos, _selectedMenuIndex==3?SEL_CLR:UNSEL_CLR, 1.0f, 0.0f, 0.0f);

	_spriteBatch->end();

	_device->present();
}

int MenuScreen::zIndex() const {
	return 0;
}