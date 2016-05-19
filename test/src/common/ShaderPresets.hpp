#ifndef __test_shaderpresets__
#define __test_shaderpresets__

#include <string>
#include <ciri/Graphics.hpp>
#include <cc/Mat4.hpp>

class SimpleShader {
public:
	__declspec(align(16))
	struct Constants {
		cc::Mat4f world;
		cc::Mat4f xform;
	};

	__declspec(align(16))
	struct MaterialConstants {
		cc::Vec3f diffuseColor;
		int hasDiffuseTexture;

		MaterialConstants() {
			hasDiffuseTexture = 0;
			diffuseColor = cc::Vec3f(1.0f, 1.0f, 1.0f);
		}
	};

public:
	SimpleShader();
	~SimpleShader();
	void* operator new( size_t i );
	void operator delete( void* p );

	bool create( std::shared_ptr<ciri::graphics::IGraphicsDevice> device );

	const std::shared_ptr<ciri::graphics::IShader>& getShader() const;
	Constants& getConstants();
	MaterialConstants& getMaterialConstants();
	bool updateConstants();

private:
	std::shared_ptr<ciri::graphics::IShader> _shader;
	std::shared_ptr<ciri::graphics::IConstantBuffer> _constantBuffer;
	Constants _constants;
	std::shared_ptr<ciri::graphics::IConstantBuffer> _materialConstantsBuffer;
	MaterialConstants _materialConstants;
};

#endif /* __test_shaderpresets__ */