#ifndef __test_grid__
#define __test_grid__

#include <vector>
#include <ciri/gfx/IGraphicsDevice.hpp>
#include <cc/Vec3.hpp>
#include <cc/Mat4.hpp>

class Grid {
private:
	struct GridVertex {
		cc::Vec3f position;
		cc::Vec3f color;
	};

	struct GridConstants {
		cc::Mat4f xform;
	};

public:
	Grid();
	~Grid();

	bool create( ciri::IGraphicsDevice* device );
	bool updateConstants( const cc::Mat4f& xform );
	void clean();
	bool isValid() const;

	void setSpacing( float val );
	void setExtents( int val );
	void setMajorSpacing( int val );
	void setAxisColor( const cc::Vec3f& val );
	void setBorderColor( const cc::Vec3f& val );
	void setGridColor( const cc::Vec3f& val );
	void setMajorColor( const cc::Vec3f& val );

	ciri::IVertexBuffer* getVertexBuffer() const;
	ciri::IShader* getShader() const;

private:
	void createVertices();
	bool createBuffers();
	bool loadShader();

private:
	ciri::IGraphicsDevice* _device;
	//
	bool _initialized;
	//
	ciri::IVertexBuffer* _vertexBuffer;
	ciri::IShader* _shader;
	ciri::IConstantBuffer* _constantBuffer;
	GridConstants _constants;
	//
	float _gridSpacing;
	int _gridExtents;
	int _majorSpacing;
	std::vector<GridVertex> _gridVertices;
	//
	cc::Vec3f _axisColor;
	cc::Vec3f _borderColor;
	cc::Vec3f _gridColor;
	cc::Vec3f _majorColor;
};

#endif /* __test_grid__ */
