#ifndef __grid__
#define __grid__

#include <cc/Vec2.hpp>
#include <ciri/game/SpriteBatch.hpp>
#include <ciri/Graphics.hpp>

struct GridPoint {
	float x;  float y;
	float ox; float oy;
	float dx; float dy;
	float fx; float fy;

	GridPoint();

	void update( const float xx, const float yy );
	void disrupt( float xx, float yy );
};
class BMGrid {
public:
	BMGrid( const int gridWidth, const int gridHeight, const int playfieldWidth, const int playfieldHeight, const std::shared_ptr<ciri::IGraphicsDevice>& device );
	~BMGrid();

	void resetAll();
	void updateGrid();
	void draw( ciri::SpriteBatch& spritebatch );

	void pull( const int x1, const int y1, const int size=4, const int amount=4 );
	void push( const int x1, const int y1, const int size=4, const int amount=4 );
	void shockwave( const int x1, const int y1 );

private:
	void drawLine( ciri::SpriteBatch& spriteBatch, const cc::Vec2f& start, const cc::Vec2f& end, const cc::Vec4f& color, float thickness=2.0f );
	__forceinline int getIndex( const int x, const int y ) const;

private:
	GridPoint* _grid;
	int _playfieldWidth;
	int _playfieldHeight;
	int _gridWidth;
	int _gridHeight;
	int _numPointsW;
	int _numPointsH;
	std::shared_ptr<ciri::ITexture2D> _pixel;
};

#endif /* __grid__ */