#ifndef __entity__
#define __entity__

#include <memory>
#include <cc/Vec2.hpp>
#include <cc/Vec4.hpp>
#include <ciri/Graphics.hpp>

class Entity {
public:
	Entity();
	virtual ~Entity();

public:
	const cc::Vec2f& getPosition() const;
	const cc::Vec2f& getVelocity() const;
	const float& getOrientation() const;
	const bool& isAlive() const;
	std::shared_ptr<ciri::graphics::ITexture2D> getTexture() const;
	const cc::Vec2f& getOrigin() const;
	const cc::Vec4f& getColor() const;
	const float& getCollisionRadius() const;

	void setPosition( const cc::Vec2f& position );
	void setVelocity( const cc::Vec2f& velocity );
	void setOrientation( const float orientation );
	void setIsAlive( const bool alive );
	void setTexture( const std::shared_ptr<ciri::graphics::ITexture2D>& texture );
	void setColor( const cc::Vec4f& color );
	void setCollisionRadius( float radius );

protected:
	cc::Vec2f _position; /**< Position on screen in pixels. */
	cc::Vec2f _velocity;  /**< Velocity. */
	float _orientation; /**< Rotation. */
	bool _isAlive; /**< Is the Entity alive or not? */

	std::shared_ptr<ciri::graphics::ITexture2D> _texture; /**< Texture to draw. */
	cc::Vec2f _origin;  /**< Pivot point of the sprite's texture. */
	cc::Vec4f _color;   /**< Color tint including transparency. */

	float _collisionRadius; /**< Radius from the position used for collision detection. */
};

#endif /* __entity__ */