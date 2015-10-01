#ifndef __bullet__
#define __bullet__

#include "Entity.hpp"

class Bullet : public Entity {
public:
	Bullet();
	virtual ~Bullet();
	void update( const cc::Vec4f& bounds );
};

#endif /* __bullet__ */