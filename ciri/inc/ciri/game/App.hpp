#ifndef __ciri_game_App__
#define __ciri_game_App__

#include <memory>
#include <string>
#include <ciri/Core.hpp>
#include <ciri/Graphics.hpp>

namespace ciri {

struct AppConfig {
	std::string title;
	int width;
	int height;
	AppConfig() {
		title = "ciri";
		width = 1280;
		height = 720;
	}
};

class App {
public:
	App();
	virtual ~App();

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
	AppConfig _config;

private:
	bool _isRunning;
	bool _isInitialized;
	bool _shouldGtfo;
	std::shared_ptr<ciri::IWindow> _window;
	std::shared_ptr<ciri::IInput> _input;
	std::shared_ptr<ciri::IGraphicsDevice> _graphicsDevice;
	std::shared_ptr<ciri::ITimer> _gameTimer;
};

}

#endif