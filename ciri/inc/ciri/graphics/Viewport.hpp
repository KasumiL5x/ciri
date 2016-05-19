#ifndef __ciri_graphics_Viewport__
#define __ciri_graphics_Viewport__

namespace ciri { namespace graphics {

class Viewport {
public:
	Viewport();
	Viewport( int x, int y, int width, int height );
	Viewport( int x, int y, int width, int height, float minDepth, float maxDepth );
	Viewport( const Viewport& rhs );
	Viewport& operator=( const Viewport& rhs );
	~Viewport();

	int x() const;
	int y() const;
	int width() const;
	int height() const;
	float minDepth() const;
	float maxDepth() const;
	float aspectRatio() const;

private:
	int _x;
	int _y;
	int _width;
	int _height;
	float _minDepth;
	float _maxDepth;
};

}}

#endif