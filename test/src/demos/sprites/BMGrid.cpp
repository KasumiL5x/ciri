#include "BMGrid.hpp"
#include <cmath>

template<typename T> int sign( T val ) {
	return (T(0) < val) - (val < T(0)); 
}

GridPoint::GridPoint() {
	x=y=ox=oy=dx=dy=fx=fy=0.0f;
}

void GridPoint::update( const float xx, const float yy ) {
	if( fabsf(xx - x) > 2.0f ) {
		dx += static_cast<float>(sign(xx - x));
	}

	if( fabsf(yy - y) > 2.0f ) {
		dy += static_cast<float>(sign(yy - y));
	}

	if( fabsf(ox - x) > 1.0f ) {
		x += static_cast<float>(sign(ox - x));
		dx += static_cast<float>(sign(ox - x)) * 0.5f;
	} else {
		x = ox;
	}

	if( fabsf(oy - y) > 1.0f ) {
		y += static_cast<float>(sign(oy - y));
		dy += static_cast<float>(sign(oy - y)) * 0.5f;
	} else {
		y = oy;
	}

	dx *= 0.899f;
	dy *= 0.899f;

	x += dx;
	y += dy;
}

void GridPoint::disrupt( float xx, float yy ) {
	if( fabsf(xx) > 8.0f ) {
		xx /= 16.0f;
	}
	if( fabsf(yy) > 8.0f ) {
		yy /= 16.0f;
	}

	dx += xx;
	dy += yy;
	
	const float speed = dx*dx + dy*dy;
	if( speed > 160.0f ) {
		dx /= speed * 128.0f;
		dy /= speed * 128.0f;
	}
}

BMGrid::BMGrid( const int gridWidth, const int gridHeight, const int playfieldWidth, const int playfieldHeight, const std::shared_ptr<ciri::IGraphicsDevice>& device ) {
	_playfieldWidth = playfieldWidth;
	_playfieldHeight = playfieldHeight;
	_gridWidth = gridWidth;
	_gridHeight = gridHeight;
	_numPointsW = playfieldWidth / gridWidth;
	_numPointsH = playfieldHeight / gridHeight;

	_grid = new GridPoint[_numPointsW * _numPointsH];

	unsigned char whitePixel[] = {255, 255, 255, 255};
	_pixel = device->createTexture2D(1, 1, ciri::TextureFormat::Color, 0, whitePixel);
}

BMGrid::~BMGrid() {
	if( _grid != nullptr ) {
		delete[] _grid;
		_grid = nullptr;
	}
}

void BMGrid::resetAll() {
	for( int i = 0; i < _numPointsW; ++i ) {
		for( int j = 0; j < _numPointsH; ++j ) {
			const int idx = getIndex(i, j);
			_grid[idx].ox = static_cast<float>(i * _gridWidth);
			_grid[idx].oy = static_cast<float>(j * _gridHeight);
			_grid[idx].x  = static_cast<float>(i * _gridWidth);
			_grid[idx].y  = static_cast<float>(j * _gridHeight);
			_grid[idx].dx = 0.0f;
			_grid[idx].dy = 0.0f;
		}
	}
}

void BMGrid::updateGrid() {
	for( int a = 1; a < (_numPointsW-1); ++a ) {
		for( int b = 1; b < (_numPointsH-1); ++b ) {
			float xx = 0.0f;
			xx += _grid[getIndex(a-1, b)].x;
			xx += _grid[getIndex(a, b-1)].x;
			xx += _grid[getIndex(a, b+1)].x;
			xx += _grid[getIndex(a+1, b)].x;
			xx /= 4.0f;

			float yy = 0.0f;
			yy += _grid[getIndex(a-1, b)].y;
			yy += _grid[getIndex(a, b-1)].y;
			yy += _grid[getIndex(a, b+1)].y;
			yy += _grid[getIndex(a+1, b)].y;
			yy /= 4.0f;

			_grid[getIndex(a, b)].update(xx, yy);
		}
	}
}

void BMGrid::draw( SpriteBatch& spritebatch ) {
	const int GRID_HILITE = 4;

	const int gw_low = 0;
	const int gh_low = 0;
	const int gw_hi = _numPointsW;
	const int gh_hi = _numPointsH;
	const int bold_w = GRID_HILITE - (gw_low % GRID_HILITE);
	const int bold_h = GRID_HILITE - (gh_low % GRID_HILITE);

	for( int a = gw_low; a < (gw_hi-1); ++a ) {
		for( int b = gh_low; b < (gh_hi-1); ++b ) {
			const float start_x = _grid[getIndex(a, b)].x;
			const float start_y = _grid[getIndex(a, b)].y;
			const float width = 2.0f;
			const float height = 2.0f;
			const float end_x = start_x + width;
			const float end_y = start_y + height;

			drawLine(spritebatch, cc::Vec2f(start_x, start_y), cc::Vec2f(end_x, end_y), cc::Vec4f(1.0f, 1.0f, 1.0f, 1.0f), 1.0f);
		}
	}

	for( int a = (gw_low + bold_w); a < (gw_hi-1); a += GRID_HILITE ) {
		for( int b = gh_low; b < (gh_hi-1); ++b ) {
			const float start_x = _grid[getIndex(a, b)].x;
			const float start_y = _grid[getIndex(a, b)].y;
			const float width = 3.0f;
			const float height = 3.0f;
			const float end_x = start_x + width;
			const float end_y = start_y + height;

			drawLine(spritebatch, cc::Vec2f(start_x, start_y), cc::Vec2f(end_x, end_y), cc::Vec4f(1.0f, 0.0f, 0.0f, 1.0f), 2.0f);
		}
	}

	for( int a = gw_low; a < (gw_hi-1); ++a ) {
		for( int b = (gh_low + bold_h); b < (gh_hi-1); b += GRID_HILITE ) {
			const int start_x = _grid[getIndex(a, b)].x;
			const int start_y = _grid[getIndex(a, b)].y;
			const float width = 3.0f;
			const float height = 3.0f;
			const float end_x = start_x + width;
			const float end_y = start_y + height;

			drawLine(spritebatch, cc::Vec2f(start_x, start_y), cc::Vec2f(end_x, end_y), cc::Vec4f(1.0f, 0.2f, 0.2f, 1.0f), 2.0f);
		}
	}
}

void BMGrid::pull( const int x1, const int y1, const int size, const int amount ) {
	const int a = x1 / _gridWidth;
	const int b = y1 / _gridHeight;

	for( int xx = -size; xx < size;  ++xx ) {
		for( int yy = -size; yy < size; ++yy ) {
			if( (a+xx) > 0 ) {
				if( (a+xx) <= _numPointsW ) {
					if( (b+yy) > 0 ) {
						if( (b+yy) <= _numPointsH ) {
							if( (xx*xx + yy*yy) < (size*size) ) {
								const float diff_x = _grid[getIndex(a+xx, b+yy)].x - static_cast<float>(x1);
								const float diff_y = _grid[getIndex(a+xx, b+yy)].y - static_cast<float>(y1);
								const float dist = sqrtf(diff_x*diff_x + diff_y*diff_y);
								if( dist > 0.0f ) {
									_grid[getIndex(a+xx, b+yy)].dx -= diff_x / dist * amount;
									_grid[getIndex(a+xx, b+yy)].dy -= diff_y / dist * amount;
								}
							}
						}
					}
				}
			}
		}
	}
}

void BMGrid::push( const int x1, const int y1, const int size, const int amount ) {
	const int a = x1 / _gridWidth;
	const int b = y1 / _gridHeight;

	for( int xx = -size; xx < size; ++xx ) {
		for( int yy = -size; yy < size; ++yy ) {
			if( (a+xx) > 0 ) {
				if( (a+xx) <= _numPointsW ) {
					if( (b+yy) > 0 ) {
						if( (b+yy) <= _numPointsH ) {
							const float diff_x = _grid[getIndex(a+xx, b+yy)].ox - static_cast<float>(x1);
							const float diff_y = _grid[getIndex(a+xx, b+yy)].oy - static_cast<float>(y1);
							const float diff_xo = _grid[getIndex(a+xx, b+yy)].ox - _grid[getIndex(a+xx, b+yy)].x;
							const float diff_yo = _grid[getIndex(a+xx, b+yy)].oy - _grid[getIndex(a+xx, b+yy)].y;
							const float dist = diff_y*diff_y + diff_x*diff_x;
							const float dist_o = diff_yo*diff_yo + diff_xo*diff_xo;
							if( dist > 1.0f && dist_o < 400.0f ) {
								_grid[getIndex(a+xx, b+yy)].dx += diff_x * amount;
								_grid[getIndex(a+xx, b+yy)].dy += diff_y * amount;
							}
						}
					}
				}
			}
		}
	}
}

void BMGrid::shockwave( const int x1, const int y1 ) {
	const int a = x1 / _gridWidth;
	const int b = y1 / _gridHeight;

	for( int xx = -3; xx < 3; ++xx ) {
		for( int yy = -3; yy < 3; ++yy ) {
			if( (xx*xx + yy*yy) < 10 ) {
				if( (a+xx) > 0 ) {
					if( (a+xx) <= _numPointsW ) {
						if( (b+yy) > 0 ) {
							if( (b+yy) <= _numPointsH ) {
								_grid[getIndex(a+xx, b+yy)].disrupt(4.0f * (_grid[getIndex(a+xx, b+yy)].x - x1), 4.0f * (_grid[getIndex(a+xx, b+yy)].y - y1));
							}
						}
					}
				}
			}
		}
	}
}

void BMGrid::drawLine( SpriteBatch& spriteBatch, const cc::Vec2f& start, const cc::Vec2f& end, const cc::Vec4f& color, float thickness ) {
	const cc::Vec2f delta = end - start;
	const float angle = atan2f(delta.y, delta.x);
	const cc::Vec2f scale = cc::Vec2f(delta.magnitude(), thickness);
	spriteBatch.draw(_pixel, start, angle, cc::Vec2f(0.0f, 0.5f), scale, 0.0f, color);
}

int BMGrid::getIndex( const int x, const int y ) const {
	return (y * _numPointsW) + x;
}