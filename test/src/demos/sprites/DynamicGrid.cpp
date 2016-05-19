#include "DynamicGrid.hpp"
#include <vector>

PointMass::PointMass()
	: position(0.0f), velocity(0.0f), inverseMass(0.0f), acceleration(0.0f), damping(0.98f) {
}

PointMass::PointMass( const cc::Vec3f& thePosition, float theInverseMass )
	: position(thePosition), velocity(0.0f), inverseMass(theInverseMass), acceleration(0.0f), damping(0.98f) {
}

void PointMass::applyForce( const cc::Vec3f& force ) {
	acceleration += force * inverseMass;
}

void PointMass::increaseDamping( float factor ) {
	damping *= factor;
}

void PointMass::update() {
	velocity += acceleration;
	position += velocity;
	acceleration = cc::Vec3f::zero();
	if( velocity.sqrMagnitude() < 0.000001f ) {
		velocity = cc::Vec3f::zero();
	}
	velocity *= damping;
	damping = 0.98f;
}

Spring::Spring()
	: end1(nullptr), end2(nullptr), stiffness(0.0f), damping(0.0f), targetLength(0.0f) {
}

Spring::Spring( PointMass* theEnd1, PointMass* theEnd2, float theStiffness, float theDamping )
	: end1(theEnd1), end2(theEnd2), stiffness(theStiffness), damping(theDamping) {
	targetLength = end1->position.distance(end2->position) * 0.95f;
}

void Spring::update() {
	// spring forces
	const cc::Vec3f springDist = end1->position - end2->position;
	const float springLen = springDist.magnitude();
	if( springLen <= targetLength ) {
		return;
	}
	const float SPRING_CONSTANT = stiffness;
	cc::Vec3f force = -SPRING_CONSTANT * (springLen - targetLength) * springDist;
	if( springLen > 0.0f ) {
		force /= springLen;
	}
	const float SPRING_DAMPING = damping;
	force -= SPRING_DAMPING * (end1->velocity - end2->velocity);
	end1->applyForce(force);
	end2->applyForce(-force);

	//cc::Vec3f x = end1->position - end2->position;
	//float length = x.magnitude();
	//if( length <= targetLength ) {
	//	return;
	//}
	//x = (x / length) * (length - targetLength);
	//const cc::Vec3f dv = end2->velocity - end1->velocity;
	//const cc::Vec3f force = stiffness * x - dv * damping;
	//end1->applyForce(-force);
	//end2->applyForce(force);
}

DynamicGrid::DynamicGrid()
	: _numColumns(0), _numRows(0), _pixel(nullptr) {
}

DynamicGrid::DynamicGrid( const cc::Vec4f& size, const cc::Vec2f& spacing, const std::shared_ptr<ciri::IGraphicsDevice>& device )
	: _numColumns(0), _numRows(0), _pixel(nullptr) {
	_numColumns = static_cast<int>(size.z / spacing.x) + 1;
	_numRows = static_cast<int>(size.w / spacing.y) + 1;
	_points.resize(_numColumns * _numRows);

	// fixed points are used to anchor the grid to fixed positions
	_fixedPoints.resize(_numColumns * _numRows);

	// create point masses
	int column=0, row=0;
	const float top = size.y;
	const float bottom = size.y + size.w;
	const float left = size.x;
	const float right = size.x + size.z;
	for( float y = top; y <= bottom; y += spacing.y ) {
		for( float x = left; x <= right; x += spacing.x ) {
			_points[row * _numColumns + column] = PointMass(cc::Vec3f(x, y, 0.0f), 1.0f);
			_fixedPoints[row * _numColumns + column] = PointMass(cc::Vec3f(x, y, 0.0f), 0.0f);
			column++;
		}
		row++;
		column = 0;
	}

	// link point masses with springs
	for( int y = 0; y < _numRows; ++y ) {
		for( int x = 0; x < _numColumns; ++x ) {
			if( 0 == x || 0 == y || (_numColumns-1) == x || (_numRows-1) == y ) {
				// anchor border of grid
				_springs.push_back(Spring(&_fixedPoints[getPixelCoord(x, y)], &_points[getPixelCoord(x, y)], 0.1f, 0.1f));
			} else if( 0 == (x % 3) || 0 == (y % 3) ) {
				// loosely anchor 1/9th of the point masses
				_springs.push_back(Spring(&_fixedPoints[getPixelCoord(x, y)], &_points[getPixelCoord(x, y)], 0.04f, 0.06f));
				//_springs.push_back(Spring(&_fixedPoints[getPixelCoord(x, y)], &_points[getPixelCoord(x, y)], 0.002f, 0.02f));
			}

			const float stiffness = 0.28f;
			const float damping = 0.06f;

			if( x > 0 ) {
				_springs.push_back(Spring(&_points[getPixelCoord(x-1, y)], &_points[getPixelCoord(x, y)], stiffness, damping));
			}

			if( y > 0 ) {
				_springs.push_back(Spring(&_points[getPixelCoord(x, y-1)], &_points[getPixelCoord(x, y)], stiffness, damping));
			}
		}
	}

	unsigned char whitePixel[] = {255, 255, 255, 255};
	_pixel = device->createTexture2D(1, 1, ciri::TextureFormat::RGBA32_UINT, 0, whitePixel);
}

void DynamicGrid::applyDirectedForce( const cc::Vec2f& force, const cc::Vec2f& position, float radius ) {
	applyDirectedForce(cc::Vec3f(force.x, force.y, 0.0f), cc::Vec3f(position.x, position.y, 0.0f), radius);
}

void DynamicGrid::applyDirectedForce( const cc::Vec3f& force, const cc::Vec3f& position, float radius ) {
	const float r2 = radius * radius;
	for( auto& mass : _points ) {
		if( position.sqrDistance(mass.position) < (r2) ) {
			mass.applyForce(10.0f * force / (10.0f + position.distance(mass.position)));
		}
	}
}

void DynamicGrid::applyImplosiveForce( float force, const cc::Vec2f& position, float radius ) {
	applyImplosiveForce(force, cc::Vec3f(position.x, position.y, 0.0f), radius);
}

void DynamicGrid::applyImplosiveForce( float force, const cc::Vec3f& position, float radius ) {
	const float r2 = radius * radius;
	for( auto& mass : _points ) {
		const float dist2 = position.sqrDistance(mass.position);
		if( dist2 < r2 ) {
			mass.applyForce(10.0f * force * (position - mass.position) / (100.0f + dist2));
			mass.increaseDamping(0.6f);
		}
	}
}

void DynamicGrid::applyExplosiveForce( float force, const cc::Vec2f& position, float radius ) {
	applyExplosiveForce(force, cc::Vec3f(position.x, position.y, 0.0f), radius);
}

void DynamicGrid::applyExplosiveForce( float force, const cc::Vec3f& position, float radius ) {
	const float r2 = radius * radius;
	for( auto& mass : _points ) {
		const float dist2 = position.sqrDistance(mass.position);
		if( dist2 < r2 ) {
			mass.applyForce(100.0f * force * (mass.position - position) / (10000.0f + dist2));
			mass.increaseDamping(0.6f);
		}
	}
}

void DynamicGrid::update() {
	for( auto& spring : _springs ) {
		spring.update();
	}

	for( auto& mass : _points ) {
		mass.update();
	}
}

void DynamicGrid::draw( ciri::SpriteBatch& spriteBatch, float scrw, float scrh ) {
	const int width = _numColumns;
	const int height = _numRows;
	const cc::Vec4f color = cc::Vec4f(0.12f, 0.12f, 0.55f, 0.33f); // dark blue

	const cc::Vec2f screenSize = cc::Vec2f(scrw, scrh);

	for( int y = 1; y < height; ++y ) {
		for( int x = 1; x < width; ++x ) {
			cc::Vec2f left;
			cc::Vec2f up;
			const cc::Vec2f p = toVec2(_points[getPixelCoord(x, y)].position, screenSize);

			if( x > 1 ) {
				left = toVec2(_points[getPixelCoord(x-1, y)].position, screenSize);
				const float thickness = y % 3 == 1 ? 3.0f : 1.0f;

				// catmul-rom to smooth bends
				const int clampedX = cc::math::minimum<int>(x+1, width-1);
				const cc::Vec2f mid = cc::math::catmullRom(toVec2(_points[getPixelCoord(x-2, y)].position, screenSize),
																									 left,
																									 p,
																									 toVec2(_points[getPixelCoord(clampedX, y)].position, screenSize),
																									 0.5f);

				// if grid is straight, draw line, otherwise draw interp midpoint
				if( mid.sqrDistance((left + p) * 0.5f) > 1.0f ) {
					drawLine(spriteBatch, left, mid, color, thickness);
					drawLine(spriteBatch, mid, p, color, thickness);
				} else {
					drawLine(spriteBatch, left, p, color, thickness);
				}
			}

			if( y > 1 ) {
				up = toVec2(_points[getPixelCoord(x, (y-1))].position, screenSize);
				const float thickness = x % 3 == 1 ? 3.0f : 1.0f;
				const int clampedY = cc::math::minimum<int>(y+1, height-1);
				const cc::Vec2f mid = cc::math::catmullRom(toVec2(_points[getPixelCoord(x, y-2)].position, screenSize),
																									 up,
																									 p,
																									 toVec2(_points[getPixelCoord(x, clampedY)].position, screenSize),
																									 0.5f);

				if( mid.sqrDistance((up + p) * 0.5f) > 1.0f ) {
					drawLine(spriteBatch, up, mid, color, thickness);
					drawLine(spriteBatch, mid, p, color, thickness);
				} else {
					drawLine(spriteBatch, up, p, color, thickness);
				}
			}

			// interp lines between point masses; makes it look denser
			if( x > 1 && y > 1 ) {
				const cc::Vec2f upLeft = toVec2(_points[getPixelCoord(x-1, y-1)].position, screenSize);
				drawLine(spriteBatch, 0.5f * (upLeft + up), 0.5f * (left + p), color, 1.0f); // vertical
				drawLine(spriteBatch, 0.5f * (upLeft + left), 0.5f * (up + p), color, 1.0f); // horizontal
			}
		}
	}
}

void DynamicGrid::drawLine( ciri::SpriteBatch& spriteBatch, const cc::Vec2f& start, const cc::Vec2f& end, const cc::Vec4f& color, float thickness ) {
	const cc::Vec2f delta = end - start;
	const float angle = atan2f(delta.y, delta.x);
	const cc::Vec2f scale = cc::Vec2f(delta.magnitude(), thickness);
	spriteBatch.draw(_pixel, start, angle, cc::Vec2f(0.0f, 0.5f), scale, 0.0f, color);
}

cc::Vec2f DynamicGrid::toVec2( const cc::Vec3f& vec, const cc::Vec2f& screenSize ) {
	// perspective projection
	const float factor = (vec.z + 2000.0f) / 2000.0f;
	return (cc::Vec2f(vec.x, vec.y) - screenSize * 0.5f) * factor + screenSize * 0.5f;
}

int DynamicGrid::getPixelCoord( int x, int y ) const {
	return y * _numColumns + x;
}