#ifndef __test_shaderpresets__
#define __test_shaderpresets__

#include <string>
#include <ciri/gfx/IGraphicsDevice.hpp>
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

	bool create( ciri::IGraphicsDevice* device, const std::string& ext );

	ciri::IShader* getShader() const;
	Constants& getConstants();
	MaterialConstants& getMaterialConstants();
	bool updateConstants();

private:
	ciri::IShader* _shader;
	ciri::IConstantBuffer* _constantBuffer;
	Constants _constants;
	ciri::IConstantBuffer* _materialConstantsBuffer;
	MaterialConstants _materialConstants;
};

#endif /* __test_shaderpresets__ */