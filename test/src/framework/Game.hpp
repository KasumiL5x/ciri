#ifndef __game__
#define __game__

#include <memory>
#include <string>
#include <ciri/Window.hpp>
#include <ciri/Input.hpp>
#include <ciri/Graphics.hpp>
#include <ciri/Core.hpp>

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
	virtual void onEvent( const ciri::WindowEvent& evt );
	virtual void onUpdate( const double deltaTime, const double elapsedTime );
	virtual void onFixedUpdate( const double deltaTime, const double elapsedTime );
	virtual void onDraw();
	virtual void onUnloadContent();
	void gtfo();

protected:
	std::shared_ptr<ciri::IWindow> window() const;
	std::shared_ptr<ciri::IInput> input() const;
	std::shared_ptr<ciri::IGraphicsDevice> graphicsDevice() const;
	std::shared_ptr<ciri::ITimer> gameTimer() const;

protected:
	GameConfig _config;

private:
	bool _isRunning;
	bool _isInitialized;
	bool _shouldGtfo;
	std::shared_ptr<ciri::IWindow> _window;
	std::shared_ptr<ciri::IInput> _input;
	std::shared_ptr<ciri::IGraphicsDevice> _graphicsDevice;
	std::shared_ptr<ciri::ITimer> _gameTimer;
};

#endif /* __game__ */