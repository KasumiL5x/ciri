#ifndef __playground_PlayerPlaneController__
#define __playground_PlayerPlaneController__

#include <ciri/core/input/IInput.hpp>
#include "PlaneController.hpp"

class PlayerPlaneController : public PlaneController {
public:
	PlayerPlaneController(  const std::shared_ptr<Plane>& plane, const std::shared_ptr<ciri::IInput>& input );
	virtual ~PlayerPlaneController();

	virtual void update( const float deltaTime, const float elapsedTime ) override;

private:
	std::shared_ptr<ciri::IInput> _input;
};

#endif