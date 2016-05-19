#include <ciri/graphics/Viewport.hpp>

using namespace ciri;

Viewport::Viewport()
	: _x(0), _y(0), _width(1280), _height(720), _minDepth(0.0f), _maxDepth(1.0f) {
}

Viewport::Viewport( int x, int y, int width, int height )
	: _x(x), _y(y), _width(width), _height(height), _minDepth(0.0f), _maxDepth(1.0f) {
}

Viewport::Viewport( int x, int y, int width, int height, float minDepth, float maxDepth )
	: _x(x), _y(y), _width(width), _height(height), _minDepth(minDepth), _maxDepth(maxDepth) {
}

Viewport::Viewport( const Viewport& rhs ) {
	this->_x = rhs._x;
	this->_y = rhs._y;
	this->_width = rhs._width;
	this->_height = rhs._height;
	this->_minDepth = rhs._minDepth;
	this->_maxDepth = rhs._maxDepth;
}

Viewport& Viewport::operator=( const Viewport& rhs ) {
	this->_x = rhs._x;
	this->_y = rhs._y;
	this->_width = rhs._width;
	this->_height = rhs._height;
	this->_minDepth = rhs._minDepth;
	this->_maxDepth = rhs._maxDepth;
	return *this;
}

Viewport::~Viewport() {
}

int Viewport::x() const {
	return _x;
}

int Viewport::y() const {
	return _y;
}

int Viewport::width() const {
	return _width;
}

int Viewport::height() const {
	return _height;
}

float Viewport::minDepth() const {
	return _minDepth;
}

float Viewport::maxDepth() const {
	return _maxDepth;
}

float Viewport::aspectRatio() const {
	return (_width != 0 && _height != 0) ? static_cast<float>(_width) / static_cast<float>(_height) : 0.0f;
}