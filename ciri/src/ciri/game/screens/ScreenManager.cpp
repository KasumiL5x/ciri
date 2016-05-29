#include <ciri/game/screens/ScreenManager.hpp>
#include <ciri/game/screens/Screen.hpp>
#include <algorithm>

using namespace ciri;

ScreenManager::ScreenManager() {
}

ScreenManager::~ScreenManager() {
	_orderedScreens.clear();
	_states.clear();
	_screenNames.clear();
}

bool ScreenManager::add( const std::shared_ptr<Screen>& screen, const std::string& name, bool activate ) {
	if( nullptr == screen ) {
		return false;
	}

	const auto existing = _screenNames.find(name);
	if( existing != _screenNames.end() ) {
		return false;
	}

	_screenNames[name] = screen;
	_orderedScreens.push_back(screen);
	_states[screen] = activate ? ScreenState::Active : ScreenState::Hidden;

	screen->onAdd();

	return true;
}

bool ScreenManager::remove( const std::string& name ) {
	const auto existing = _screenNames.find(name);
	if( _screenNames.end() == existing ) {
		return false;
	}

	existing->second->onRemove();

	_states.erase(existing->second);
	_orderedScreens.erase(std::find(_orderedScreens.begin(), _orderedScreens.end(), existing->second));
	_screenNames.erase(name);
	return true;
}

bool ScreenManager::remove( const std::shared_ptr<Screen>& screen ) {
	if( nullptr == screen ) {
		return false;
	}

	const auto name = std::find_if(_screenNames.begin(), _screenNames.end(), [screen](const std::pair<std::string, std::shared_ptr<Screen>>& pair){
		return pair.second == screen;
	});

	if(_screenNames.end() == name ) {
		return false;
	}

	return remove(name->first);
}

void ScreenManager::update( double deltaTime, double elapsedTime ) {
	// sort the screens by z-index
	std::sort(_orderedScreens.begin(), _orderedScreens.end(), [](const std::shared_ptr<Screen>& lhs, const std::shared_ptr<Screen>& rhs ) -> bool {
		return lhs->zIndex() > rhs->zIndex();
	});

	// copy ordered screens to avoid removal during update issues
	const std::vector<std::shared_ptr<Screen>> loop = _orderedScreens;

	for( const auto& curr : loop ) {
		if( ScreenState::Active == _states[curr] ) {
			curr->onUpdate(deltaTime, elapsedTime);
		}
	}
}

 void ScreenManager::updateFixed( double deltaTime, double elapsedTime ) {
	const std::vector<std::shared_ptr<Screen>> loop = _orderedScreens;
	 for( const auto& curr : loop ) {
		 if( ScreenState::Active == _states[curr] ) {
			 curr->onFixedUpdate(deltaTime, elapsedTime);
		 }
	 }
 }

 void ScreenManager::draw() {
	 const std::vector<std::shared_ptr<Screen>> loop = _orderedScreens;
	 for( const auto& curr : loop ) {
		 if( ScreenState::Active == _states[curr] || ScreenState::Inactive == _states[curr] ) {
			 curr->onDraw();
		 }
	 }
 }

 bool ScreenManager::activate( const std::string& name ) {
	 const auto existing = _screenNames.find(name);
	 if( _screenNames.end() == existing ) {
		 return false;
	 }
	 return activate(existing->second);
 }

bool ScreenManager::activate( const std::shared_ptr<Screen>& screen ) {
	const auto existing = _states.find(screen);
	if( _states.end() == existing || ScreenState::Active == existing->second ) {
		return false;
	}

	existing->second = ScreenState::Active;
	return true;
}

bool ScreenManager::deactivate( const std::string& name ) {
	const auto existing = _screenNames.find(name);
	if( _screenNames.end() == existing ) {
		return false;
	}
	return deactivate(existing->second);
}

bool ScreenManager::deactivate( const std::shared_ptr<Screen>& screen ) {
	const auto existing = _states.find(screen);
	if( _states.end() == existing || ScreenState::Inactive == existing->second ) {
		return false;
	}
	existing->second = ScreenState::Inactive;
	return true;
}

bool ScreenManager::hide( const std::string& name ) {
	const auto existing = _screenNames.find(name);
	if( _screenNames.end() == existing ) {
		return false;
	}
	return hide(existing->second);
}

bool ScreenManager::hide( const std::shared_ptr<Screen>& screen ) {
	const auto existing = _states.find(screen);
	if( _states.end() == existing || ScreenState::Hidden == existing->second ) {
		return false;
	}
	existing->second = ScreenState::Hidden;
	return true;
}