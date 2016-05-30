#include "PlayerPlaneController.hpp"

PlayerPlaneController::PlayerPlaneController( const std::shared_ptr<Plane>& plane , const std::shared_ptr<ciri::IInput>& input )
	: PlaneController(plane), _input(input) {
}

PlayerPlaneController::~PlayerPlaneController() {
}

void PlayerPlaneController::update( const float deltaTime, const float elapsedTime ) {
	const float speed = 300.0f * deltaTime;
	const float pitchAmount = 1.0f * deltaTime;
	const float rollAmount = 1.0f * deltaTime;
	
	if( true || _input->isKeyDown(ciri::Key::Space) ) {
		_plane->throttle(speed);
	}
	if( _input->isKeyDown(ciri::Key::W) ) {
		_plane->pitch(pitchAmount);
	}
	if( _input->isKeyDown(ciri::Key::S) ){
		_plane->pitch(-pitchAmount);
	}
	if( _input->isKeyDown(ciri::Key::A) ) {
		_plane->roll(rollAmount);
	}
	if( _input->isKeyDown(ciri::Key::D) ) {
		_plane->roll(-rollAmount);
	}
}