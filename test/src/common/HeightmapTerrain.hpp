#ifndef __heightmap_terrain__
#define __heightmap_terrain__

#include <cc/Vec2.hpp>
#include <cc/Vec3.hpp>
#include <cc/Vec4.hpp>
#include <ciri/gfx/IGraphicsDevice.hpp>
#include <ciri/util/TGA.hpp>

class HeightmapTerrain {
private:
	struct TerrainVertex {
		cc::Vec3f position;
		cc::Vec3f normal;
		cc::Vec2f texcoord;
	};

public:
	HeightmapTerrain();
	~HeightmapTerrain();

	bool generate( const ciri::TGA& heightmap, ciri::IGraphicsDevice* device );
	void clean();

	ciri::IVertexBuffer* getVertexBuffer() const;
	ciri::IIndexBuffer* getIndexBuffer() const;

private:
	void boxFilterHeightData( unsigned long width, unsigned long height, float*& heightMap, bool smoothEdges );

private:
	float* _heightData;
	TerrainVertex* _vertices;
	int* _indices;
	ciri::IVertexBuffer* _vertexBuffer;
	ciri::IIndexBuffer* _indexBuffer;
};

#endif /* __heightmap_terrain__ */
