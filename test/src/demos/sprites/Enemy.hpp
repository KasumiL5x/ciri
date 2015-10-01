#ifndef __enemy__
#define __enemy__

#include "Entity.hpp"

class Enemy : public Entity {
public:
	Enemy();
	virtual ~Enemy();

	void update( const cc::Vec2f& screenSize );

	void setTarget( const std::shared_ptr<Entity>& target );

private:
	void followTarget( const float acceleration );

private:
	std::shared_ptr<Entity> _target;

public:
	static Enemy createSeeker( const cc::Vec2f& position );
};

#endif /* __enemy__ */