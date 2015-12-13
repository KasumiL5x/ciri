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

	const std::shared_ptr<ciri::IVertexBuffer>& getVertexBuffer() const;
	const std::shared_ptr<ciri::IShader>& getShader() const;
	bool isValid() const;

	bool create( float length, std::shared_ptr<ciri::IGraphicsDevice> device );
	bool updateConstants( const cc::Mat4f& xform );

	void clean();

private:
	std::shared_ptr<ciri::IVertexBuffer> _vertexBuffer;
	std::shared_ptr<ciri::IShader> _shader;
	std::shared_ptr<ciri::IConstantBuffer> _constantsBuffer;
	AxisConstants _constants;
};

#endif /* __test_axis__ */