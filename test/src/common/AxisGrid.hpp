#ifndef __axis_grid__
#define __axis_grid__

#include <memory>
#include <ciri/Graphics.hpp>
#include <cc/Vec3.hpp>
#include <cc/Mat4.hpp>

class AxisGrid {
private:
	struct GridVertex {
		cc::Vec3f position;
		cc::Vec3f color;
	};
	struct GridConstants {
		cc::Mat4f xform;
	};

public:
	AxisGrid();
	~AxisGrid();

	bool create( std::shared_ptr<ciri::IGraphicsDevice> device );
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

	const std::shared_ptr<ciri::IVertexBuffer>& getVertexBuffer() const;
	const std::shared_ptr<ciri::IShader>& getShader() const;

private:
	void createVertices();
	bool createBuffers();
	bool loadShader();

private:
	std::shared_ptr<ciri::IGraphicsDevice> _device;
	//
	bool _initialized;
	//
	std::shared_ptr<ciri::IVertexBuffer> _vertexBuffer;
	std::shared_ptr<ciri::IShader> _shader;
	std::shared_ptr<ciri::IConstantBuffer> _constantBuffer;
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

#endif /* __ciri_game_axis_grid__ */