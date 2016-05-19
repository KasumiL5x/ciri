#ifndef __ciri_game_axis_widget__
#define __ciri_game_axis_widget__

#include <memory>
#include <cc/Vec3.hpp>
#include <cc/Mat4.hpp>
#include <ciri/Graphics.hpp>

class AxisWidget {
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
	AxisWidget();
	~AxisWidget();

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

#endif /* __ciri_game_axis_widget__ */