#ifndef __heightmap_terrain__
#define __heightmap_terrain__

#include <cc/Vec2.hpp>
#include <cc/Vec3.hpp>
#include <cc/Vec4.hpp>
#include <cc/Mat4.hpp>
#include <ciri/gfx/IGraphicsDevice.hpp>
#include <ciri/util/TGA.hpp>

class HeightmapTerrain {
private:
	struct TerrainVertex {
		cc::Vec3f position;
		cc::Vec3f normal;
		cc::Vec4f tangent;
		cc::Vec2f texcoord;
		cc::Vec4f texweights;
	};

	_declspec(align(16))
	struct PerFrameConstants {
		cc::Mat4f world;
		cc::Mat4f xform;
	};

public:
	HeightmapTerrain();
	~HeightmapTerrain();

	bool generate( const ciri::TGA& heightmap, ciri::IGraphicsDevice* device );
	void setTextures( ciri::ITexture2D* tex0, ciri::ITexture2D* tex1, ciri::ITexture2D* tex2, ciri::ITexture2D* tex3 );
	void draw( const cc::Mat4f& viewProj, ciri::IGraphicsDevice* device );
	void clean();
	
private:
	void updateConstants( const cc::Mat4f& world, const cc::Mat4f& viewProj );
	void boxFilterHeightData( unsigned long width, unsigned long height, float*& heightMap, bool smoothEdges );
	std::string getVertexShaderGl() const;
	std::string getVertexShaderDx() const;
	std::string getPixelShaderGl() const;
	std::string getPixelShaderDx() const;

private:
	bool _generated;
	
	// raw buffers
	float* _heightData;
	TerrainVertex* _vertices;
	int* _indices;

	// gpu buffers
	ciri::IVertexBuffer* _vertexBuffer;
	ciri::IIndexBuffer* _indexBuffer;

	// shader
	ciri::IShader* _shader;

	// constant buffers
	ciri::IConstantBuffer* _perFrameConstantBuffer;
	PerFrameConstants _perFrameConstants;

	// textures
	ciri::ITexture2D* _textures[4];
	ciri::ISamplerState* _sampler;
};

#endif /* __heightmap_terrain__ */
