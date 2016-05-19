#ifndef __game__
#define __game__

#include <memory>
#include <string>
#include <ciri/Core.hpp>
#include <ciri/Graphics.hpp>

struct GameConfig {
	std::string title;
	int width;
	int height;
	GameConfig() {
		title = "ciri";
		width = 1280;
		height = 720;
	}
};

class Game {
public:
	Game();
	virtual ~Game();

public:
	bool run();

protected:
	virtual void onInitialize();
	virtual void onLoadContent();
	virtual void onEvent( const ciri::core::WindowEvent& evt );
	virtual void onUpdate( const double deltaTime, const double elapsedTime );
	virtual void onFixedUpdate( const double deltaTime, const double elapsedTime );
	virtual void onDraw();
	virtual void onUnloadContent();
	void gtfo();

protected:
	std::shared_ptr<ciri::core::IWindow> window() const;
	std::shared_ptr<ciri::core::IInput> input() const;
	std::shared_ptr<ciri::graphics::IGraphicsDevice> graphicsDevice() const;
	std::shared_ptr<ciri::core::ITimer> gameTimer() const;

protected:
	GameConfig _config;

private:
	bool _isRunning;
	bool _isInitialized;
	bool _shouldGtfo;
	std::shared_ptr<ciri::core::IWindow> _window;
	std::shared_ptr<ciri::core::IInput> _input;
	std::shared_ptr<ciri::graphics::IGraphicsDevice> _graphicsDevice;
	std::shared_ptr<ciri::core::ITimer> _gameTimer;
};

#endif /* __ciri_game_game__ */