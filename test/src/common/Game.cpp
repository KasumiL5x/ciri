#include "Game.hpp"

Game::Game()
	: _isRunning(false), _isInitialized(false), _shouldGtfo(false), _window(nullptr), _input(nullptr),
		_graphicsDevice(nullptr), _gameTimer(nullptr) {
}

Game::~Game() {
}

bool Game::run() {
	// cannot run two times
	if( _isRunning ) {
		printf("ciri error: Failed to run because already running.\n");
		return false;
	}

	// cannot run if already initialized
	if( _isInitialized ) {
		printf("ciri error: Failed to run because already initialized.\n");
		return false;
	}

	// create window
	_window = ciri::createWindow();
	if( !_window->create(_config.width, _config.height) ) {
		printf("ciri error: Failed to run because window creation failed.\n");
		return false;
	}
	_window->setWindowText(_config.title.c_str());

	// create input
	_input = ciri::createInput();
	if( !_input->create(_window) ) {
		printf("ciri error: Failed to run because input creation failed.\n");
		return false;
	}

	// create graphics device
	_graphicsDevice = ciri::createGraphicsDevice();
	if( !_graphicsDevice->create(_window) ) {
		printf("ciri error: Failed to run because graphics device creation failed.\n");
		return false;
	}

	// create game timer
	_gameTimer = ciri::createTimer();

	onInitialize();
	onLoadContent();

	// start game timer
	_gameTimer->start();

	const double MS_PER_UPDATE = 1.0 / 60.0;
	double elapsedTime = 0.0;
	double lastTime = 0.0;
	double lag = 0.0;
	_isRunning = true;
	while( _isRunning && !_shouldGtfo ) {
		ciri::WindowEvent evt;
		while( _window->pollEvent(evt) ) {
			onEvent(evt);
		}

		const double currTime = _gameTimer->getElapsedMicrosecs();
		const double deltaTime = (currTime - lastTime) * 0.000001;
		lastTime = currTime;
		elapsedTime += deltaTime;
		lag += deltaTime;

		if( !_input->poll() ) {
			printf("ciri warning: Failed to poll input.\n");
		}

		onUpdate(deltaTime, elapsedTime);

		while( lag >= MS_PER_UPDATE ) {
			onFixedUpdate(MS_PER_UPDATE, elapsedTime);
			lag -= MS_PER_UPDATE;
		}

		if( !_input->update() ) {
			printf("ciri warning: Failed to update input.\n");
		}

		onDraw();
	}

	onUnloadContent();

	// clean resources
	_input = nullptr;
	_gameTimer = nullptr;
	_graphicsDevice->destroy();
	_graphicsDevice = nullptr;
	_window->close();
	_window = nullptr;

	return true;
}

void Game::onInitialize() {
}

void Game::onLoadContent() {
}

void Game::onEvent( const ciri::WindowEvent& evt ) {
	// window closed event check
	if( ciri::WindowEvent::Closed == evt.type ) {
		gtfo();
		return;
	}
}

void Game::onUpdate( const double deltaTime, const double elapsedTime ) {
}

void Game::onFixedUpdate( const double deltaTime, const double elapsedTime ) {
}

void Game::onDraw() {
}

void Game::onUnloadContent() {
}

void Game::gtfo() {
	_shouldGtfo = true;
}

std::shared_ptr<ciri::IWindow> Game::window() const {
	return _window;
}

std::shared_ptr<ciri::IInput> Game::input() const {
	return _input;
}

std::shared_ptr<ciri::IGraphicsDevice> Game::graphicsDevice() const {
	return _graphicsDevice;
}

std::shared_ptr<ciri::ITimer> Game::gameTimer() const {
	return _gameTimer;
}