#ifndef __playground_PlaneController__
#define __playground_PlaneController__

#include <memory>
#include "Plane.hpp"

class PlaneController {
public:
	PlaneController( const std::shared_ptr<Plane>& plane  )
		: _plane(plane) {
	}
	virtual ~PlaneController() {
	}

	virtual void update( const float deltaTime, const float elapsedTime )=0;

protected:
	std::shared_ptr<Plane> _plane;
};

#endif