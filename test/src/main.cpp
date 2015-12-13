#include <crtdbg.h>
#include <memory>
#include <ciri/Window.hpp>
#include <ciri/Graphics.hpp>
#include <ciri/Input.hpp>
#include <ciri/Core.hpp>
#include "demos/dynvb/DynamicVertexBufferDemo.hpp"
#include "demos/terrain/TerrainDemo.hpp"
#include "demos/sprites/SpritesDemo.hpp"
#include "demos/refract/RefractDemo.hpp"
//#include "demos/deferred/DeferredDemo.hpp"

enum class Demo {
	Dynvb,
	Terrain,
	Sprites,
	Refract,
	//Deferred
};

std::unique_ptr<Game> createGame( Demo type ) {
	switch( type ) {
		case Demo::Dynvb: {
			return std::unique_ptr<Game>(new DynamicVertexBufferDemo());
		}

		case Demo::Terrain:{ 
			return std::unique_ptr<Game>(new TerrainDemo());
		}

		case Demo::Sprites: {
			return std::unique_ptr<Game>(new SpritesDemo());
		}

		case Demo::Refract: {
			return std::unique_ptr<Game>(new RefractDemo());
		}

		//case Demo::Deferred: {
		//	return std::unique_ptr<IDemo>(new DeferredDemo());
		//}

		default: {
			return nullptr;
		}
	}
}

int main() {
	// enable memory leak checking
#ifdef _DEBUG
	int debugFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	debugFlag|= _CRTDBG_LEAK_CHECK_DF;
	debugFlag |= _CRTDBG_CHECK_ALWAYS_DF;
	_CrtSetDbgFlag(debugFlag);
#endif

	// create the game
	std::unique_ptr<Game> game = createGame(Demo::Refract);
	if( !game->run() ) {
		printf("ciri error: Game failed to run!\n");
	}

	// clean the game
	game = nullptr;

	return 0;
}