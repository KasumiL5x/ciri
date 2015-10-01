#ifndef __player__
#define __player__

#include "Entity.hpp"

class PlayerShip : public Entity {
public:
	PlayerShip();
	virtual ~PlayerShip();

	void update( const cc::Vec2f& movement );
};

#endif /* __player__ */