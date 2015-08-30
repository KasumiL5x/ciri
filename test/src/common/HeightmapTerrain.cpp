#include "HeightmapTerrain.hpp"

HeightmapTerrain::HeightmapTerrain()
	: _generated(false), _heightData(nullptr), _vertices(nullptr), _indices(nullptr), _vertexBuffer(nullptr), _indexBuffer(nullptr) {
}

HeightmapTerrain::~HeightmapTerrain() {
}

bool HeightmapTerrain::generate( const ciri::TGA& heightmap, ciri::IGraphicsDevice* device ) {
	if( _generated ) {
		return false;
	}

	// get pixels from heightmap
	const unsigned char* pixels = heightmap.getPixels();
	if( nullptr == pixels ) {
		return false;
	}

	// get heightmap size
	const int width = heightmap.getWidth();
	const int height = heightmap.getHeight();
	if( width <= 0 || height <= 0 ) {
		return false;
	}
	float minHeight = std::numeric_limits<float>::max();
	float maxHeight = std::numeric_limits<float>::min();
	// create height data array
	const int BPP = (ciri::TGA::RGB == heightmap.getFormat()) ? 3 : 4;
	_heightData = new float[width * height];
	for( int x = 0; x < width; ++x ) {
		for( int y = 0; y < height; ++y ) {
			// get height value from heightmap image
			_heightData[y * width + x] = pixels[(y * width + x) * BPP]; // red component

			// update min and max heights
			minHeight = (_heightData[y * width + x] < minHeight) ? _heightData[y * width + x] : minHeight;
			maxHeight = (_heightData[y * width + x] > maxHeight) ? _heightData[y * width + x] : maxHeight;
		}
	}
	// normalize heights
	const float SCALE = 200.0f;
	const float INV_SCALE = 1.0f / SCALE;
	for( int x = 0; x < width; ++x ) {
		for( int y = 0; y < height; ++y ) {
			_heightData[y * width + x] = (_heightData[y * width + x] - minHeight) / (maxHeight - minHeight) * SCALE;
		}
	}

	// smooth heightmap
	boxFilterHeightData(width, height, _heightData, true);

	// create vertices
	const float halfWidth = static_cast<float>(width) * 0.5f;
	const float invWidth = 1.0f / static_cast<float>(width);
	const float halfHeight = static_cast<float>(-height) * 0.5f;
	const float invHeight =  1.0f / static_cast<float>(-height);
	const int vertexCount = width * height;
	_vertices = new TerrainVertex[vertexCount];
	for( int x = 0; x < width; ++x ) {
		for( int y = 0; y < height; ++y ) {
			const float fx = static_cast<float>(x);
			const float fy = static_cast<float>(-y);

			// position of vertex w/ the center of the terrain as the origin
			const float xpos = ((fx * invWidth) * 2.0f - 1.0f) * halfWidth;
			const float ypos = _heightData[y * width + x];
			const float zpos = ((fy * invHeight) * 2.0f - 1.0f) * halfHeight;
			_vertices[y * width + x].position = cc::Vec3f(xpos, ypos, zpos);

			_vertices[y * width + x].texcoord.x = fx * INV_SCALE;
			_vertices[y * width + x].texcoord.y = fy * INV_SCALE;

			// for a corner as the origin, use:
			//_vertices[y * width + x].position = cc::Vec3f(static_cast<float>(x), _heightData[y * width + x], static_cast<float>(-y));

			// todo: weights for multitexture
		}
	}
	
	// create indices
	const int indexCount = (width-1) * (height-1) * 6;
	_indices = new int[indexCount];
	int counter = 0;
	for( int y = 0; y < (height-1); ++y ) {
		for( int x = 0; x < (width-1); ++x ) {
			const int lowerLeft = y * width + x;
			const int lowerRight = y * width + (x + 1);
			const int topLeft = (y + 1) * width + x;
			const int topRight = (y + 1) * width + (x + 1);

			// CCW
			_indices[counter++] = topLeft;
			_indices[counter++] = lowerLeft;
			_indices[counter++] = lowerRight;
			//
			_indices[counter++] = topLeft;
			_indices[counter++] = lowerRight;
			_indices[counter++] = topRight;

			// CW
			//_indices[counter++] = topLeft;
			//_indices[counter++] = lowerRight;
			//_indices[counter++] = lowerLeft;
			//
			//_indices[counter++] = topLeft;
			//_indices[counter++] = topRight;
			//_indices[counter++] = lowerRight;
		}
	}

	// calculate vertex normals
	for( int i = 0; i < (indexCount / 3); ++i ) {
		const int index1 = _indices[i * 3];
		const int index2 = _indices[i * 3 + 1];
		const int index3 = _indices[i * 3 + 2];

		const cc::Vec3f side1 = _vertices[index1].position - _vertices[index3].position;
		const cc::Vec3f side2 = _vertices[index1].position - _vertices[index2].position;

		// CCW
		const cc::Vec3f normal = side2.cross(side1);

		// CW
		//const cc::Vec3f normal = side1.cross(side2);

		_vertices[index1].normal += normal;
		_vertices[index2].normal += normal;
		_vertices[index3].normal += normal;
	}
	// normalize vertex normals
	for( int i = 0; i < vertexCount; ++i ) {
		_vertices[i].normal.normalize();
	}

	// create gpu buffers
	_vertexBuffer = device->createVertexBuffer();
	_vertexBuffer->set(_vertices, sizeof(TerrainVertex), vertexCount, false);
	_indexBuffer = device->createIndexBuffer();
	_indexBuffer->set(_indices, indexCount, false);

	_generated = true;

	return true;
}

void HeightmapTerrain::clean() {
	if( !_generated ) {
		return;
	}

	if( _indices != nullptr ) {
		delete[] _indices;
		_indices = nullptr;
	}

	if( _vertices != nullptr ) {
		delete[] _vertices;
		_vertices = nullptr;
	}

	if( _heightData != nullptr ) {
		delete[] _heightData;
		_heightData = nullptr;
	}

	_generated = false;
}

ciri::IVertexBuffer* HeightmapTerrain::getVertexBuffer() const {
	return _vertexBuffer;
}

ciri::IIndexBuffer* HeightmapTerrain::getIndexBuffer() const {
	return _indexBuffer;
}

void HeightmapTerrain::boxFilterHeightData( unsigned long width, unsigned long height, float*& heightMap, bool smoothEdges ) {
	// http://archive.gamedev.net/archive/reference/articles/article2164.html
	// i did not write this, and i do not dare touch it

  // Temporary values for traversing single dimensional arrays
  long x = 0;
  long z = 0;
  
  long  widthClamp = (smoothEdges) ?  width : width  - 1;
  long heightClamp = (smoothEdges) ? height : height - 1;
  
  // [Optimization] Calculate bounds ahead of time
  unsigned int bounds = width * height;
  
  // Validate requirements
  if (!heightMap)
    return;
  
  // Allocate the result
  float* result = new float[bounds];
  
  // Make sure memory was allocated
  if (!result)
    return;
  
  for (z = (smoothEdges) ? 0 : 1; z < heightClamp; ++z)
  {
    for (x = (smoothEdges) ? 0 : 1; x < widthClamp; ++x)
    {
      // Sample a 3x3 filtering grid based on surrounding neighbors
      
      float value = 0.0f;
      float cellAverage = 1.0f;
      
      // Sample top row
      
      if (((x - 1) + (z - 1) * width) >= 0 &&
          ((x - 1) + (z - 1) * width) < bounds)
      {
        value += heightMap[(x - 1) + (z - 1) * width];
        ++cellAverage;
      }
      
      if (((x - 0) + (z - 1) * width) >= 0 &&
          ((x - 0) + (z - 1) * width) < bounds)
      {
        value += heightMap[(x    ) + (z - 1) * width];
        ++cellAverage;
      }
      
      if (((x + 1) + (z - 1) * width) >= 0 &&
          ((x + 1) + (z - 1) * width) < bounds)
      {
        value += heightMap[(x + 1) + (z - 1) * width];
        ++cellAverage;
      }
      
      // Sample middle row
      
      if (((x - 1) + (z - 0) * width) >= 0 &&
          ((x - 1) + (z - 0) * width) < bounds)
      {
        value += heightMap[(x - 1) + (z    ) * width];
        ++cellAverage;
      }
      
      // Sample center point (will always be in bounds)
      value += heightMap[x + z * width];
      
      if (((x + 1) + (z - 0) * width) >= 0 &&
          ((x + 1) + (z - 0) * width) < bounds)
      {
        value += heightMap[(x + 1) + (z    ) * width];
        ++cellAverage;
      }
      
      // Sample bottom row
      
      if (((x - 1) + (z + 1) * width) >= 0 &&
          ((x - 1) + (z + 1) * width) < bounds)
      {
        value += heightMap[(x - 1) + (z + 1) * width];
        ++cellAverage;
      }
      
      if (((x - 0) + (z + 1) * width) >= 0 &&
          ((x - 0) + (z + 1) * width) < bounds)
      {
        value += heightMap[(x    ) + (z + 1) * width];
        ++cellAverage;
      }
      
      if (((x + 1) + (z + 1) * width) >= 0 &&
          ((x + 1) + (z + 1) * width) < bounds)
      {
        value += heightMap[(x + 1) + (z + 1) * width];
        ++cellAverage;
      }
      
      // Store the result
      result[x + z * width] = value / cellAverage;
    }
  }
  
  // Release the old array
  delete [] heightMap;
  
  // Store the new one
  heightMap = result;
}