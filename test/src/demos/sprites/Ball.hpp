#ifndef __ball__
#define __ball__

#include <memory>
#include <ciri/Graphics.hpp>
#include <cc/Vec2.hpp>

class Ball {
public:
	Ball();
	~Ball();

	/**
	 * Gets the associated texture.
	 * @returns Texture assigned to this Ball.
	 */
	std::shared_ptr<ciri::ITexture2D> getTexture() const;

	/**
	 * Sets the texture to draw with and re-centers the origin.
	 * @param texture Texture to set.
	 * @returns True upon success; false otherwise.
	 */
	bool setTexture( const std::shared_ptr<ciri::ITexture2D>& texture );

	/**
	 * Gets the rotation used for drawing.
	 * @returns Rotation in radians.
	 */
	float getRotation() const;

	/**
	 * Sets the rotation used for drawing.
	 * @param rotation New rotation in radians.
	 */
	void setRotation( float rotation );

	/**
   * Gets the origin used for drawing.  This is the "zeroed" translation and rotation pivot.
	 * @returns Origin in pixels relative to the bottom-left of the image where {texw/2, texh/2} is the center.
	 */
	const cc::Vec2f& getOrigin() const;

	/**
	 * Gets the position of the Ball.
	 * @returns Position of the Ball.
	 */
	const cc::Vec2f& getPosition() const;

	/**
	 * Sets the position of the ball.
	 * @param position New position of the Ball.
	 */
	void setPosition( const cc::Vec2f& position );

	/**
	 * Gets the velocity of the Ball.
	 * @returns Velocity of the Ball.
	 */
	const cc::Vec2f& getVelocity() const;

	/**
	 * Sets the velocity of the Ball.
	 * @param velocity New velocity of the Ball.
	 */
	void setVelocity( const cc::Vec2f& velocity );

	/**
	 * Gets if the Ball is active.  Can be used to decide whether to run logic, draw, etc.
	 * @returns True if the Ball is active; false otherwise.
	 */
	bool isActive() const;

	/**
	 * Sets if the Ball is active.
	 * @param active New active state.
	 */
	void setActive( bool active );

private:
	std::shared_ptr<ciri::ITexture2D> _texture; /**< Texture used to draw. */
	float _rotation; /**< Rotation used for drawing. */
	cc::Vec2f _origin; /**< Origin of rotation used for drawing. */

	cc::Vec2f _position; /**< Current position. */
	cc::Vec2f _velocity; /**< Current velocity. */

	bool _active; /**< Is this ball active or not? */
};

#endif /* __ball__ */