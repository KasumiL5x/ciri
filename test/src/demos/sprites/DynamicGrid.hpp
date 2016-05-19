#ifndef __grid__
#define __grid__

//http://gamedevelopment.tutsplus.com/tutorials/make-a-neon-vector-shooter-in-xna-the-warping-grid--gamedev-9904

#include <vector>
#include <cc/Vec2.hpp>
#include <cc/Vec3.hpp>
#include <cc/Vec4.hpp>
#include <ciri/Graphics.hpp>
#include <ciri/game/SpriteBatch.hpp>

struct PointMass {
	cc::Vec3f position;
	cc::Vec3f velocity;
	float inverseMass;
	cc::Vec3f acceleration;
	float damping;

	PointMass();
	PointMass( const cc::Vec3f& thePosition, float theInverseMass );
	void applyForce( const cc::Vec3f& force );
	void increaseDamping( float factor );
	void update();
};

struct Spring {
	PointMass* end1;
	PointMass* end2;
	float targetLength;
	float stiffness;
	float damping;

	Spring();
	Spring( PointMass* theEnd1, PointMass* theEnd2, float theStiffness, float theDamping );
	void update();
};

class DynamicGrid {
public:
	DynamicGrid();
	DynamicGrid( const cc::Vec4f& size, const cc::Vec2f& spacing, const std::shared_ptr<ciri::IGraphicsDevice>& device );

	void applyDirectedForce( const cc::Vec2f& force, const cc::Vec2f& position, float radius );
	void applyDirectedForce( const cc::Vec3f& force, const cc::Vec3f& position, float radius );
	void applyImplosiveForce( float force, const cc::Vec2f& position, float radius );
	void applyImplosiveForce( float force, const cc::Vec3f& position, float radius );
	void applyExplosiveForce( float force, const cc::Vec2f& position, float radius );
	void applyExplosiveForce( float force, const cc::Vec3f& position, float radius );
	void update();
	void draw( ciri::SpriteBatch& spriteBatch, float scrw, float scrh );

private:
	void drawLine( ciri::SpriteBatch& spriteBatch, const cc::Vec2f& start, const cc::Vec2f& end, const cc::Vec4f& color, float thickness=2.0f );
	cc::Vec2f toVec2( const cc::Vec3f& vec, const cc::Vec2f& screenSize );
	__forceinline	int getPixelCoord( int x, int y ) const;

private:
	std::vector<Spring> _springs;
	std::vector<PointMass> _points;
	std::vector<PointMass> _fixedPoints;
	int _numColumns;
	int _numRows;
	std::shared_ptr<ciri::ITexture2D> _pixel;
};

#endif /* __grid__ */