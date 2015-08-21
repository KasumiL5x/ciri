#ifndef __test_axis__
#define __test_axis__

#include <string>
#include <cc/Vec3.hpp>
#include <cc/Mat4.hpp>
#include <ciri/gfx/IGraphicsDevice.hpp>

class Axis {
private:
	struct AxisVertex {
		cc::Vec3f position;
		cc::Vec3f color;

		AxisVertex() {
		}
		AxisVertex( const cc::Vec3f& pos, const cc::Vec3f& clr )
			: position(pos), color(clr) {
		}
	};

	struct AxisConstants {
		cc::Mat4f xform;
	};

public:
	Axis();
	~Axis();

	ciri::IVertexBuffer* getVertexBuffer() const;
	ciri::IShader* getShader() const;
	bool isValid() const;

	bool create( float length, ciri::IGraphicsDevice* device );
	bool updateConstants( const cc::Mat4f& xform );

private:
	ciri::IVertexBuffer* _vertexBuffer;
	ciri::IShader* _shader;
	ciri::IConstantBuffer* _constantsBuffer;;
	AxisConstants _constants;
};

#endif /* __test_axis__ */